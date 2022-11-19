// Panduza
#include "CfgTree.hpp"
#include "Store/Store.hpp"

// Qt
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// ============================================================================
//
CfgTree::CfgTree(QObject* parent)
    : QObject(parent)
    , _machine(tr("New Machine"))
{ }

// ============================================================================
//
bool CfgTree::loadFromWorkspace()
{
    // Try to open the save file
    auto load_filepath = Store::Get().workspace.cfgTreeJsonFilePath();
    QFile load_file(load_filepath);
    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load tree.json file.");
        return false;
    }
    loadFromString(load_file.readAll());
    return true;
}

// ============================================================================
//
void CfgTree::loadFromString(const QByteArray& data)
{

    QJsonDocument doc = QJsonDocument::fromJson(data);
    auto obj = doc.object();

    loadFromJson(obj);

}

// ============================================================================
//
void CfgTree::loadFromJson(const QJsonObject& obj)
{
    auto brokers = obj["brokers"].toObject() ;
    // qDebug() << brokers;

    for( const auto& broker_name : brokers.keys() )
    {
        auto instance = insertBroker(false);
        instance->setName(broker_name);
        instance->loadFromJson(brokers[broker_name].toObject());
    }

    // Sync the saved model
    copyCurrentOnSavedModel();

    // Notify
    emit updated();
    emit updatedFromOutside();
}

// ============================================================================
//
bool CfgTree::saveToWorkspace()
{
    // Create the container directory before
    auto dirpath = Store::Get().workspace.etcPzaDirPath();
    if( !QDir().mkpath( dirpath ) )
    {
        _errString = tr("Unable to create directory %1").arg(dirpath);
        return false;
    }

    // Try to open the save file
    auto path = Store::Get().workspace.cfgTreeJsonFilePath();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        _errString = tr("Unable to open file %1 (%2)").arg(path).arg(file.errorString());
        return false;
    }

    // Convert data into json
    QJsonObject obj;
    saveToJson(obj);

    // Write into the file
    file.write(QJsonDocument(obj).toJson());

    // Everything end well
    return true;
}

// ============================================================================
//
void CfgTree::saveToJson(QJsonObject& obj)
{
    // Machine name
    obj["machine"] = _machine;

    // Brokers
    QJsonObject b_dict;
    for(const auto& b : _brokers)
    {
        QJsonObject b_obj;
        b->saveToJson(b_obj);
        b_dict[b->name()] = b_obj;
    }
    obj["brokers"] = b_dict;

    // Sync the saved model
    copyCurrentOnSavedModel();

    // Notify
    emit updated();
}

// ============================================================================
//
CfgBroker::ShPtr CfgTree::insertBroker(bool emitSignal)
{
    // 
    auto new_broker = CfgBroker::ShPtr(new CfgBroker());
    connect(new_broker.data(), &CfgBroker::updated, [this](){ emit updated(); });

    // Append the new broker ti the internal data
    _brokers << new_broker;

    // alert the rest of the application
    if (emitSignal) emit updated();

    // 
    return new_broker;
}

// ============================================================================
//
bool CfgTree::isBrockerNameExist(const QString& name)
{
    return false;
}

// ============================================================================
//
bool CfgTree::needToBeSaved()
{
    // Check machine name
    if(_machine != _machineSaved) return true;

    // Quick check on broker size
    if(_brokers.size() != _brokersSaved.size()) return true;

    // Check brokers one by one
    auto it_a = _brokers.constBegin();
    auto it_b = _brokersSaved.constBegin();
    for(int i=0; i<_brokers.size(); i++)
    {
        // Check if brokers are equals
        if( (**it_a)!=(**it_b) )
        {
            return true;
        }

        // Move forward
        it_a++;
        it_b++;
    }

    return false;
}

// ============================================================================
//
void CfgTree::copyCurrentOnSavedModel()
{
    // Copy machine name
    _machineSaved = _machine;

    // Copy brokers
    _brokersSaved.clear();
    for(const auto& b : _brokers)
    {
        auto copy = CfgBroker::ShPtr(new CfgBroker(*b));
        _brokersSaved << copy;
    }
}
