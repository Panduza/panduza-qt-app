#pragma once

// Qt
#include <QList>
#include <QObject>

// Panduza
#include "ConnectionInfo.hpp"

/**
 * @brief Contain the user server list
 * 
 * The server library own server info instances.
 * The rest of the application use copy of those instances.
 *
 * The server list is ordered with info ids.
 */
class ConnectionLibrary : public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief Construct a new Server Info object
         * 
         * @param parent @see QObject
         */
        ConnectionLibrary(QObject* parent = nullptr);

        /**
         * @brief Create a New ConnectionInfo object
         * 
         * @return the pointer to the newly created connection
         */
        ConnectionInfo::ShPtr createNewConnection();

        /**
         * @brief 
         * 
         * @return the connection after the one deleted in the list
         */
        ConnectionInfo::ShPtr deleteConnection(int id);

        /**
         * @brief  Get the server at the give index
         * 
         * @param idx 
         * @return const ConnectionInfo::ShPtr& 
         */
        ConnectionInfo::ShPtr connection(int idx);

        /**
         * @brief Return the names of the servers
         * 
         * @return QStringList 
         */
        QStringList names() const;

        /**
         * @brief Check if the given name is already in the library
         * 
         * @param name 
         * @param exception (skip an index in the check)
         * @return true 
         * @return false 
         */
        bool nameAlreadyExist(const QString& name, int exception = -1);

    public slots:

        /**
         * @brief Update the server from the given info 
         *
         * Use the id from info to know which server in the library to update
         * 
         * @param info 
         */
        void updateTheGivenServer(const ConnectionInfo& info);

    signals:

        /**
         * @brief Emitted when the server list is modified
         */
        void updated();

    private:

        /**
         * @brief Find the server from the index
         * 
         * @param idx 
         * @return ConnectionInfo::ShPtr 
         */
        ConnectionInfo::ShPtr findServer(int idx);

        /**
         * @brief Reset the index of each connection to order them without gap
         */
        void restoreIndexCoherency();

        /**
         * @brief Save server info into user settings
         */
        void exportToUserSettings();

        /**
         * @brief Restore the library from the user settings saved on the system
         */
        void importFromUserSettings();

        /**
         * @brief List of the managed servers
         */
        QList<ConnectionInfo::ShPtr> mServers;

};


