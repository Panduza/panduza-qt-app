// Panduza
#include "CfgBroker.hpp"

// Qt
#include <QJsonArray>

// ============================================================================
// 
CfgBroker::CfgBroker()
    : QObject()
    , _name(tr(default_broker_name))
{ }

// ============================================================================
// 
CfgBroker::CfgBroker(const CfgBroker& from, QObject* parent)
    : QObject(parent)
{
    // Copy the name
    _name = from._name;
    _addr = from._addr;
    _port = from._port;

    // Copy brokers
    _interfaces.clear();
    for(const auto& i : from._interfaces)
    {
        insertInterface(_interfaces.size(), new CfgInterface(*i));
    }
}

// ============================================================================
// 
void CfgBroker::loadFromJson(const QJsonObject& obj)
{
    // Base info
    _addr = obj["addr"].toString();
    _port = obj["port"].toString();

    // Interfaces
    auto iface_array = obj["interfaces"].toArray();
    for(const auto& iface : iface_array)
    {
        auto instance = insertInterface(_interfaces.size());
        instance->setJsonObject(iface.toObject());
    }
}

// ============================================================================
// 
void CfgBroker::saveToJson(QJsonObject& obj)
{
    // Base info
    obj["addr"] = _addr;
    obj["port"] = _port;

    // Interfaces
    QJsonArray iface_array;
    for(const auto& iface : _interfaces)
    {
        iface_array.append(iface->jsonObject());
    }
    obj["interfaces"] = iface_array;
}

// ============================================================================
// 
CfgInterface::ShPtr CfgBroker::insertInterface(int row, CfgInterface* instance, bool signal_forward)
{
    // Create the interface
    CfgInterface* new_ = instance;
    if(instance == nullptr)
    {
        new_ = new CfgInterface();
    }
    auto new_iface = CfgInterface::ShPtr(new_);

    // Connect if required
    if(signal_forward)
    {
        connect(new_iface.data(), &CfgInterface::updated, [this](){ emit updated(); });
    }

    // Insert interface and alert the application
    _interfaces.insert(row, new_iface);
    emit updated();
    return new_iface;
}

// ============================================================================
//
bool CfgBroker::operator==(const CfgBroker& other)
{
    // Check name
    if(_name != other._name) return false;
    if(_addr != other._addr) return false;
    if(_port != other._port) return false;

    // Quick check on sizes
    if(_interfaces.size() != other._interfaces.size()) return false;

    // Check interfaces
    auto it_a = _interfaces.constBegin();
    auto it_b = other._interfaces.constBegin();
    for(int i=0; i<_interfaces.size(); i++)
    {
        // Check if brokers are equals
        if( (**it_a) != (**it_b) )
        {
            return false;
        }

        // Move forward
        it_a++;
        it_b++;
    }


    return true;
}

