#pragma once

// std
#include <atomic>

// Qt
#include <QObject>
#include <QQueue>
#include <QMutex>

// Panduza
#include <Store/connection/ConnectionInfo.hpp>
#include "SystemWorker.hpp"

/**
 * @brief Object that control the the admin connection
 * @addtogroup StoreConnectionModule
 * 
 * Admin connection is the interface to perform admin operations on the system
 * 
 * - Installm/Uninstall tools
 * - Platform Configuration
 * 
 * The object use workers to perform the operations
 * Workers can be for local or remote system
 * 
 */
class AdminConnection
    : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new object
     * @param parent @see QObject
     */
    AdminConnection(QObject* parent = nullptr)
        : QObject(parent)
        , _working(false)
        , _errString("init")
    { }

    /**
     * @brief Getter for the connection status
     * 
     * @return true the connection is ready
     * @return false else
     */
    bool isReady() const { return _worker ? _worker->isReady() : false; }

    /**
     * @brief Getter for the error string when connection not ready
     */
    const QString& errString() { return _errString; }

    /**
     * @brief Set the System Worker
     * 
     * @param worker pointer to the worker (instanciated in the store)
     */
    void setSystemWorker(SystemWorker* worker = nullptr);

    /**
     * @brief Get the Next System Message object
     * 
     * @param msg 
     * @return true if message has been found
     * @return false else
     */
    bool popSystemMessage(SystemMessage& msg)
    {
        if(!_worker) return false;
        return _worker->popSystemMessage(msg);
    }


    void execInstallSshServer();
    void execUninstallSshServer();

public slots:

    void restart();
    void stop();
    void start();

signals:

    /**
     * @brief Emitted when the connection status change
     */
    void statusChanged();

    /**
     * @brief Emitted when a system message from the worker process send a message
     * 
     * Once triggered, call @link SystemWorker::popSystemMessage @endlink to get the messages
     */
    void newSystemMessageReady();

private:

    /**
     * @brief Set the error string and trigger the status signal
     */
    void setErrString(const QString& err = tr("No Problem"));


    void concurrentRun(std::function<void(void)> function);


    /**
     * @brief Mutex to prevent 2 admin process executions at the same time
     */
    QMutex _execMutex;

    /**
     * @brief If true the connection is already performing an action
     */
    std::atomic_bool _working;

    /**
     * @brief Message explaining why the ssh connection is down
     */
    QString _errString;

    /**
     * @brief Object that perform the actual work on local or remote system
     */
    SystemWorker* _worker;

};

