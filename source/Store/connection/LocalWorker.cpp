#include "LocalWorker.hpp"
#include <Store/Store.hpp>

// Qt
#include <QDir>
#include <QPair>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QThread>


#define AS_USER false
#define AS_SUDO true

#define CATCH_STREAMS true
#define DO_NOT_CATCH_STREAMS false

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
void copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    QDir dir_dst(dst);
    if (! dir_dst.exists()) dir_dst.mkpath(dst);


    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        qDebug() << "mkpath " << dst_path;
        if( ! dir.mkpath(dst_path) ) 
        {
            qDebug() << "fail mkpath";
        }
        copyPath(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        if( ! QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f) )
        {
            qDebug() << "fail copy " <<  src + QDir::separator() + f << "   " << dst + QDir::separator() + f;
        }
    }
}

// ============================================================================
//
void LocalWorker::copyWorkspacePlatformToRemote()
{

    cmdAsSudo("mkdir -p /etc/panduza");
    cmdAsSudo("chown -R $USER /etc/panduza");
    cmdAsSudo("chmod 760 /etc/panduza");
    cmdAsSudo("rm -rf /etc/panduza/*");

    qDebug() << Store::Get().workspace.path() + QDir::separator() + "etcpza" << " >>>> " << "/etc/panduza";

    copyPath(Store::Get().workspace.path() + QDir::separator() + "etcpza", "/etc/panduza");


}

// ============================================================================
//
void LocalWorker::execSshServerInstall()
{
    cmdAsSudo("apt-get install -y openssh-server");
    // QThread::sleep(2);
    cmdAsSudo("systemctl enable ssh");
    cmdAsSudo("systemctl start ssh");
    cmdAsSudo("systemctl status ssh");
}

// ============================================================================
//
void LocalWorker::execSshServerUninstall()
{
    cmdAsSudo("apt-get remove -y openssh-server");
}

// ============================================================================
//
void LocalWorker::execInstallDocker()
{

    cmdAsSudo("apt-get update");
    cmdAsSudo("apt-get install ca-certificates curl gnupg lsb-release");
    cmdAsSudo("mkdir -p /etc/apt/keyrings");
    cmdAsSudo("curl -fsSL https://download.docker.com/linux/ubuntu/gpg | gpg --yes --dearmor -o /etc/apt/keyrings/docker.gpg");

    cmdAsSudo("echo \"deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable\" | tee /etc/apt/sources.list.d/docker.list > /dev/null");
    
    cmdAsSudo("apt-get update");
    cmdAsSudo("apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin docker-compose");

}

// ============================================================================
//
void LocalWorker::execStartPlatform()
{

    if( !QFile("/etc/systemd/system/panduza.service").exists() )
    {
        qDebug() << "create the service file";

        cmdAsSudo("touch /etc/systemd/system/panduza.service");
        cmdAsSudo("chown $USER /etc/systemd/system/panduza.service");
        cmdAsSudo("chmod 760 /etc/systemd/system/panduza.service");

        // 
        QFile file(":rsc/files/panduza.service");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QByteArray content = file.readAll();

                // 
        QFile ofile("/etc/systemd/system/panduza.service");
        if (!ofile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        ofile.write(content);
    }

    cmdAsSudo("systemctl start panduza");
    cmdAsSudo("systemctl status panduza");
}

// ============================================================================
//
void LocalWorker::execStopPlatform()
{

    cmdAsSudo("systemctl stop panduza");
    cmdAsSudo("systemctl status panduza");
}

// ============================================================================
//
void LocalWorker::execAutodetectPlatform()
{
    execute("docker pull ghcr.io/panduza/panduza-py-platform:latest", AS_USER, CATCH_STREAMS, 1000000);
    execute("docker pull ghcr.io/panduza/panduza-cxx-platform:latest", AS_USER, CATCH_STREAMS, 1000000);

    // execute("echo $AUTODETECT", AS_USER, CATCH_STREAMS);
    execute("docker-compose --project-directory /etc/panduza up", AS_USER, CATCH_STREAMS, 10000000,
        { {"AUTODETECT", "1"} }
    );



    qDebug() << "/etc/panduza/platform" << " >>>>>> " << Store::Get().workspace.path() + QDir::separator() + "etcpza" + QDir::separator() + "platform";

    copyPath("/etc/panduza/platform", Store::Get().workspace.path() + QDir::separator() + "etcpza" + QDir::separator() + "platform");


}

// ============================================================================
//
void LocalWorker::restart()
{
    if( Store::Get().connection.active.localPassword().isEmpty() )
    {
        // qDebug() << "Password empty stop !";
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


// ============================================================================
//
int LocalWorker::execute(const QString& command, bool as_sudo, bool catch_streams, int timeout_ms, const QList<QPair<QString, QString> >& envs)
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

    // Inject environment variables
    QProcessEnvironment env_vars = QProcessEnvironment::systemEnvironment();
    for(auto e : envs)
    {
        env_vars.insert(e.first, e.second);
    }
    process.setProcessEnvironment(env_vars);

    if(as_sudo)
    {
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
    }
    else
    {
        auto cmd_install_ssh_server = 
            QStringList()
            << "-c"
            << command;
        process.start("bash", cmd_install_ssh_server);
    }

  
    if (!process.waitForFinished(timeout_ms))
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

