// Panduza
#include "Store.hpp"

// std
#include <assert.h>

// Qt
#include <QDebug>

// ============================================================================
//
Store::Store()
    : QObject()
{

    
    connect(&connection.active, &ConnectionInfo::updated, this, &Store::loadAdminWorker);
    connect(&connection.active, &ConnectionInfo::updated, &connection.library, &ConnectionLibrary::updateTheGivenServer);
    connect(&connection.active, &ConnectionInfo::updated, &connection.admin, &AdminConnection::restart);


    // connect(&connection.ssh, &SshConnection::statusChanged, &platform.tree, &Tree::importFromSsh);


}

// ============================================================================
//
void Store::open()
{
    // Load active connection with the first server
    connection.active.copyFrom( *connection.library.connection(0) );
}

// ============================================================================
//
void Store::loadAdminWorker()
{
    switch (connection.active.hostLocation())
    {
    case HostLocationLocal:
        connection.admin.setSystemWorker(&worker.local);
        break;

    case HostLocationRemote:
        break;

    default:
        break;
    }
}
