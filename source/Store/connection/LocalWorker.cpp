#include "LocalWorker.hpp"
#include <Store/Store.hpp>

// Qt
#include <QProcess>
#include <QDebug>
#include <QThread>


// ============================================================================
//
bool LocalWorker::execAdminPasswordCheck()
{
    auto ret = cmdAsSudo("echo test");
    if(ret == 0)
    {
        return true;
    }
    return false;
}

// ============================================================================
//
void LocalWorker::execInstallSshServer()
{
    cmdAsSudo("apt-get install -y openssh-server");
    // QThread::sleep(2);
    cmdAsSudo("systemctl enable ssh");
    cmdAsSudo("systemctl start ssh");
    cmdAsSudo("systemctl status ssh");
}

// ============================================================================
//
void LocalWorker::execUninstallSshServer()
{
    cmdAsSudo("apt-get remove -y openssh-server");
}

// ============================================================================
//
void LocalWorker::restart()
{
    if( Store::Get().connection.active.localPassword().isEmpty() )
    {
        qDebug() << "Password empty stop !";
        updateReadyFlag(false);
        return;
    }

    if( !execAdminPasswordCheck() )
    {
        qDebug() << "Password baddd !";
        updateReadyFlag(false);
        return;
    }

    qDebug() << "okay !";
    updateReadyFlag(true);
}

// ============================================================================
//
void LocalWorker::stop()
{

}

// ============================================================================
//
void LocalWorker::start()
{

}

// ============================================================================
//
int LocalWorker::cmdAsSudo(const QString& command, bool catch_streams)
{
    QProcess process;

    if(catch_streams)
    {
        // Connect to stream events
        connect(&process, &QProcess::readyReadStandardError, [this, &process](){
            pushSystemMessage(SystemMessage(SystemStreamStdErr, process.readAllStandardError()));
        });
        connect(&process, &QProcess::readyReadStandardOutput, [this, &process](){
            pushSystemMessage(SystemMessage(SystemStreamStdOut, process.readAllStandardOutput()));
        });

        // Start event
        pushSystemMessage(SystemMessage(SystemStreamBegCmd, command.toUtf8()));
    }

    // -S => to get the pass from the echo pipe
    // -k => to bypass the sudo cache and foget the password after the command
    QString cmd_sudo = QString("echo %1 | sudo -S -k %2").arg(Store::Get().connection.active.localPassword()).arg(command);

    // !!! Warning this debug log is dangerous !!!
    // qDebug() << "[[[" << cmd_sudo << "]]]";

    auto cmd_install_ssh_server = 
        QStringList()
        << "-c"
        << cmd_sudo;
    process.start("bash", cmd_install_ssh_server);
    if (!process.waitForFinished())
    {
        pushSystemMessage(SystemMessage(SystemStreamEndCmd, "error"));
    }
    else
    {
        pushSystemMessage(SystemMessage(SystemStreamEndCmd, "ok"));
    }


    process.disconnect();

    return process.exitCode();
}

