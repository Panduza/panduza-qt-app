#pragma once

// Qt
#include <QObject>

// Panduza
#include "workspace/Workspace.hpp"

#include "connection/ConnectionInfo.hpp"
#include "connection/ConnectionLibrary.hpp"
#include "connection/SshConnection.hpp"

#include "connection/AdminConnection.hpp"
#include "connection/LocalWorker.hpp"


/**
 * @defgroup ModuleStore Store
 * @addtogroup ModuleStore
 * 
 * @brief Shared data object for the application
 * 
 * The is the main singleton that gather data that must be shared across all the view of the application
 *
 */
class Store : public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief Singleton Instance Getter
         */
        static Store& Get() {
            static Store Instance;
            return Instance;
        }

        /**
         * @brief Workspace object
         */
        Workspace workspace;

        /**
         * @brief Connections store
         */
        struct {
            ConnectionInfo active;
            ConnectionLibrary library;

            // mqtt user
            
            AdminConnection admin;
        } connection;


        struct {
            LocalWorker local;

            // SshConnection ssh; => into worker
            
        } worker;




        /**
         * @brief Start store interactions with the application
         */
        void open();

    private:

        /**
         * @brief Construct a new Store object
         * 
         * Private because singleton
         */
        Store();

    private slots:

        /**
         * @brief 
         */
        void loadAdminWorker();

};

