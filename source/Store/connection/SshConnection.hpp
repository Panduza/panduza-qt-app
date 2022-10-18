#pragma once

// std
#include <atomic>

// ssh
#include <libssh/libssh.h>

// Qt
#include <QObject>

// Panduza
#include <Store/connection/ConnectionInfo.hpp>

/**
 * @brief Manage the ssh connection
 * @addtogroup StoreConnectionModule
 * 
 * The only instance of this class should be inside the Store
 */
class SshConnection
    : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Ssh Connection object
     * 
     * @param parent 
     */
    SshConnection(QObject* parent = nullptr)
        : QObject(parent)
        , mSshSession(nullptr)
        , mErrString("init")
        , mReady(false)
        , mWorking(false)
        , mRestartFlag(false)
    { }

    /**
     * @brief Getter for the error string
     */
    const QString& errString() { return mErrString; }

    /**
     * @brief 
     * 
     * @return true
     * @return false
     */
    bool isReady() const { return mReady; }



    int remoteExec(const QString& cmd, QByteArray& stdout);
    bool isRemoteFileExist(const QString& remote_path);


    int getFile(const QString& remote_path, QByteArray& content);

public slots:

    /**
     * @brief Request a restart of the ssh connection
     * 
     * The real restart won't be performed immediatly, it wait for other operation to end.
     */
    void requestRestart();

signals:

    /**
     * @brief Emitted when the connection status change
     */
    void statusChanged();

private slots:

    /**
     * @brief Try to execute the next pending action
     */
    void nextWork();

private:

    /**
     * @brief Set the error string and trigger the status signal
     */
    void setErrString(const QString& err = tr("No Problem"));

    /**
     * @brief Restart the ssh connection on a separate thread
     */
    void asyncRestart();

    /**
     * @brief Restart the ssh connection
     */
    void restart();

    /**
     * @brief Proper stop and clear the connection
     */
    void stop();

    /**
     * @brief Start the connection and try to connect to the host
     */
    void start();

    /**
     * @brief 
     * 
     * @param session 
     * @return int 
     */
    int verifyKnownhost(ssh_session session);

    /**
     * @brief Object to manage a ssh session
     */
    ssh_session mSshSession;

    /**
     * @brief Message explaining why the ssh connection is down
     */
    QString mErrString;

    /**
     * @brief If true the connection is ready to communicate with the host
     * 
     */
    std::atomic_bool mReady;

    /**
     * @brief If true the connection is already performing an action
     */
    std::atomic_bool mWorking;

    /**
     * @brief If true the connection should restart when possible
     */
    std::atomic_bool mRestartFlag;

};

