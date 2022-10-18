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
 * @brief 
 * @addtogroup StoreConnectionModule
 * 
 */
class LocalWorker
    : public SystemWorker
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Ssh Connection object
     * 
     * @param parent 
     */
    LocalWorker(QObject* parent = nullptr)
        : SystemWorker(parent)
    { }

    /**
     * @brief Getter for the error string
     */
    // const QString& errString() { return mErrString; }

    /**
     * @brief 
     * 
     * @return true
     * @return false
     */
    // bool isReady() const { return mReady; }



    // int remoteExec(const QString& cmd, QByteArray& stdout);
    // bool isRemoteFileExist(const QString& remote_path);


    // int getFile(const QString& remote_path, QByteArray& content);





    bool execAdminPasswordCheck();


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
    // void statusChanged();

    void newSystemMessageReady();

private:

    /**
     * @brief This function the given as sudo on the system
     * 
     * @param command: the function string
     * @param catch_streams: if true catch stdout and stderr to trigger newSystemMessageReady
     * @return int 
     */
    int cmdAsSudo(const QString& command, bool catch_streams = true);



};

