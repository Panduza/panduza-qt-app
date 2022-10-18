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
void AdminConnection::execInstallSshServer()
{

    concurrentRun(std::bind(&SystemWorker::execInstallSshServer, _worker));

}

// ============================================================================
//
void AdminConnection::execUninstallSshServer()
{


    concurrentRun(std::bind(&SystemWorker::execUninstallSshServer, _worker));
 
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
void AdminConnection::concurrentRun(std::function<void(void)> worker_operation)
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
        [this]() {
            _working = false;
            qDebug() << "LEAVE";

            _execMutex.unlock();
            // === UNLOCK
    });
}
