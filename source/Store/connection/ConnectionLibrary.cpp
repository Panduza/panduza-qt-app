// Panduza
#include "ConnectionLibrary.hpp"

// Qt
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>

// ============================================================================
//
ConnectionLibrary::ConnectionLibrary(QObject* parent)
    : QObject(parent)
{
    // Try to import user settings
    importFromUserSettings();

    // If there is nothing to import from user settings, jsut create a first server
    if(mServers.empty())
    {
        createNewConnection();
    }
}

// ============================================================================
//
ConnectionInfo::ShPtr ConnectionLibrary::createNewConnection()
{
    // Get next index (push back)
    auto new_index = mServers.size();

    // Get unique name
    int idx = 0;
    while( nameAlreadyExist( tr("Unamed %1").arg(idx) ) )
    {
        idx++;
    }

    // Create the new server
    auto new_srv = ConnectionInfo::ShPtr(new ConnectionInfo(new_index));
    new_srv->setName( tr("Unamed %1").arg(idx) );
    mServers << new_srv;

    // Signal to server
    exportToUserSettings();
    emit updated();
    return new_srv;
}

// ============================================================================
//
ConnectionInfo::ShPtr ConnectionLibrary::deleteConnection(int id)
{
    // Remove
    mServers.removeAt(id);

    restoreIndexCoherency();

    return mServers.first();
}

// ============================================================================
//
ConnectionInfo::ShPtr ConnectionLibrary::connection(int idx)
{
    return findServer(idx);
}

// ============================================================================
//
QStringList ConnectionLibrary::names() const
{
    QStringList names;
    for( auto srv : mServers )
    {
        names << srv->name();
    }
    return names;
}

// ============================================================================
//
bool ConnectionLibrary::nameAlreadyExist(const QString& name, int exception)
{
    for( auto srv : mServers )
    {
        if( srv->id() == exception )
            continue;

        if( name == srv->name() )
            return true;
    }
    return false;
}

// ============================================================================
//
void ConnectionLibrary::updateTheGivenServer(const ConnectionInfo& info)
{
    auto srv = findServer(info.id());
    if(srv)
    {
        srv->copyFrom(info);
        emit updated();
        exportToUserSettings();
    }
}

// ============================================================================
//
ConnectionInfo::ShPtr ConnectionLibrary::findServer(int idx)
{
    ConnectionInfo::ShPtr found(nullptr);
    for( auto srv : mServers )
    {
        if( srv->id() == idx )
        {
            found = srv;
            break;
        }
    }
    return found;
}

// ============================================================================
//
void ConnectionLibrary::restoreIndexCoherency()
{
    int id = 0;
    for( auto srv : mServers )
    {
        srv->setId(id);
        id++;
    }
}

// ============================================================================
//
void ConnectionLibrary::exportToUserSettings()
{
    // Logs
    // qDebug() << "exportToUserSettings";

    //
    QJsonArray server_library;
    for( auto srv : mServers )
    {
        QJsonObject obj;
        srv->exportToJson(obj);
        server_library.append(obj);
    }
    QSettings settings;
    settings.setValue("server/library", server_library);
}

// ============================================================================
//
void ConnectionLibrary::importFromUserSettings()
{
    // Logs
    qDebug() << "importFromUserSettings";

    //
    mServers.clear();

    // 
    QSettings settings;
    auto server_library = settings.value("server/library").toJsonArray();
    for( auto srv : server_library )
    {
        // Create the new server
        auto new_srv = ConnectionInfo::ShPtr(new ConnectionInfo());
        new_srv->importFromJson(srv.toObject());
        mServers << new_srv;
    }
}

