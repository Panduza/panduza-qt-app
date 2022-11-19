#pragma once

// std
#include <atomic>

// Qt
#include <QQueue>
#include <QMutex>
#include <QObject>

// Panduza
#include <Store/connection/ConnectionInfo.hpp>


enum SystemStream { SystemStreamBegCmd, SystemStreamEndCmd, SystemStreamStdErr, SystemStreamStdOut };

struct SystemMessage
{
    SystemMessage(SystemStream stream = SystemStreamStdOut, const QByteArray& data = QByteArray())
        : stream(stream)
        , data(data)
    { }
    SystemStream stream;
    QByteArray data;
};


/**
 * @brief 
 * @addtogroup StoreConnectionModule
 * 
 */
class SystemWorker
    : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Ssh Connection object
     * 
     * @param parent 
     */
    SystemWorker(QObject* parent)
        : QObject(parent)
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
    bool isReady() const { return _ready; }


    void updateReadyFlag(bool new_val) 
    {
        if(new_val != _ready) { _ready = new_val; emit statusChanged(); }
    }


    // int remoteExec(const QString& cmd, QByteArray& stdout);
    // bool isRemoteFileExist(const QString& remote_path);


    // int getFile(const QString& remote_path, QByteArray& content);



    virtual void copyWorkspacePlatformToRemote() = 0;

    virtual void execSshServerInstall() = 0;
    virtual void execSshServerUninstall() = 0;

    virtual void execInstallDocker() = 0;

    virtual void execStartPlatform() = 0;
    virtual void execStopPlatform() = 0;
    virtual void execAutodetectPlatform() = 0;


    bool popSystemMessage(SystemMessage& msg)
    {
        if(_systemMessages.isEmpty()) return false;
        msg = _systemMessages.dequeue();
        return true;
    }


    void pushSystemMessage(const SystemMessage& msg)
    {
        _systemMessages.enqueue(msg);
        emit newSystemMessageReady();
    }


public slots:

    virtual void restart() = 0;
    virtual void stop() = 0;
    virtual void start() = 0;


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

private slots:

    /**
     * @brief Try to execute the next pending action
     */
    // void nextWork();

protected:

    bool _enable;


    /**
     * @brief If true the connection is ready to communicate with the host
     */
    std::atomic_bool _ready;




    QQueue<SystemMessage> _systemMessages;


};

