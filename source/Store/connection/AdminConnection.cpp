// Panduza
#include "AdminConnection.hpp"

// Qt
#include <QDebug>
#include <QtConcurrent>

// ============================================================================
//
void AdminConnection::setSystemWorker(SystemWorker* worker)
{
    _worker = worker;
    connect(_worker, &SystemWorker::statusChanged, [this](){ emit statusChanged(); });
    connect(_worker, &SystemWorker::newSystemMessageReady, [this](){ emit newSystemMessageReady(); });
}

// ============================================================================
//
void AdminConnection::copyWorkspacePlatformToRemote()
{
    concurrentRun(std::bind(&SystemWorker::copyWorkspacePlatformToRemote, _worker));
}

// ============================================================================
//
void AdminConnection::execSshServerInstall()
{
    concurrentRun(std::bind(&SystemWorker::execSshServerInstall, _worker));
}

// ============================================================================
//
void AdminConnection::execSshServerUninstall()
{
    concurrentRun(std::bind(&SystemWorker::execSshServerUninstall, _worker));
}

// ============================================================================
//
void AdminConnection::execInstallDocker()
{
    concurrentRun(std::bind(&SystemWorker::execInstallDocker, _worker));
}

// ============================================================================
//
void AdminConnection::execStartPlatform()
{
    concurrentRun(std::bind(&SystemWorker::execStartPlatform, _worker));
}

// ============================================================================
//
void AdminConnection::execStopPlatform()
{
    concurrentRun(std::bind(&SystemWorker::execStopPlatform, _worker));
}

// ============================================================================
//
void AdminConnection::execAutodetectPlatform(std::function<void(void)> then_operation)
{
    concurrentRun(std::bind(&SystemWorker::execAutodetectPlatform, _worker), then_operation);
}

// ============================================================================
//
void AdminConnection::restart()
{
    concurrentRun(std::bind(&SystemWorker::restart, _worker));

}

// ============================================================================
//
void AdminConnection::stop()
{

}

// ============================================================================
//
void AdminConnection::start()
{

}

// ============================================================================
//
void AdminConnection::concurrentRun(std::function<void(void)> worker_operation, std::function<void(void)> then_operation)
{
    // Start the connection
    auto future = QtConcurrent::run(
        [this, worker_operation] {
            // === LOCK
            _execMutex.lock();

            qDebug() << "START";
            _working = true;

            if(!_worker){
                qDebug() << "no worker";
                return;
            }
            
            worker_operation();
    }).then(
        [this, then_operation]() {
            _working = false;
            qDebug() << "LEAVE";

            _execMutex.unlock();
            // === UNLOCK

            then_operation();
    });
}
