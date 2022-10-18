// Panduza
#include "TabInterfaces.hpp"
#include <Store/Store.hpp>

// #include "PzaTreeWidget/PzaTreeTreeModel.hpp"


// Qt
#include <QDebug>
#include <QTreeView>

// ============================================================================
//
TabInterfaces::TabInterfaces(QWidget *parent)
    : QWidget(parent)
{

            // auto b = new QPushButton("test");
            // connect(b, &QPushButton::clicked, [](){
            //     // Check if /etc/panduza exist => panduza not installed ?
            //     // Check if tree.json is present, create it ?
            //     // Uninstall from the server ?
                
                
            //     // if ( Store::Get().connection.ssh.isRemoteFileExist("/etc/panduza/tree.json") )
            //     // {
            //     //     qDebug() << "pok";
            //     //     // Store::Get().connection.ssh.getFile("/etc/panduza/tree.json");
            //     // }
            // });

            // auto b2 = new QPushButton("check if panduza up");
            // connect(b2, &QPushButton::clicked, [](){
                
            //     // auto bash_line = QString("systemctl status panduza.service");
            //     // QByteArray stdout;
            //     // Store::Get().connection.ssh.remoteExec(bash_line, stdout);
            //     // qDebug() << stdout;

            //     // QString ret(stdout);

            //     // if( ret.indexOf("Active: inactive (dead)") != -1 )
            //     // {
            //     //     qDebug() << "panduza down !";
            //     // }
            //     // if( ret.indexOf("Active: active") != -1 )
            //     // {
            //     //     qDebug() << "panduza UP !";
            //     // }
            // });

            // auto b21 = new QPushButton("start");
            // connect(b21, &QPushButton::clicked, [](){

            //     // auto bash_line = QString("echo %1 | sudo -S systemctl start panduza.service").arg(Store::Get().connection.active.sshPassword());
            //     // qDebug() << bash_line;
            //     // QByteArray stdout;
            //     // Store::Get().connection.ssh.remoteExec(bash_line, stdout);
            //     // qDebug() << stdout;   
            // });

            // auto b22 = new QPushButton("stop");
            // connect(b22, &QPushButton::clicked, [](){
            //     // auto bash_line = QString("echo %1 | sudo -S systemctl stop panduza.service").arg(Store::Get().connection.active.sshPassword());
            //     // qDebug() << bash_line;
            //     // QByteArray stdout;
            //     // Store::Get().connection.ssh.remoteExec(bash_line, stdout);
            //     // qDebug() << stdout;   
            // });

            // auto b3 = new QPushButton("import tree");
            // connect(b3, &QPushButton::clicked, [](){


                
            // });



            // auto b4 = new QPushButton("install ssh server");
            // connect(b4, &QPushButton::clicked, []() {
            //     // Store::admin::install_server_ssh
            // });

            // auto lay = new QVBoxLayout(this);
            // lay->addWidget(b);
            // lay->addWidget(b2);
            // lay->addWidget(b21);
            // lay->addWidget(b22);
            // lay->addWidget(b3);


}

