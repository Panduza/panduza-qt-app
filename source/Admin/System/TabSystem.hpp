#pragma once

// Qt
#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>

// Panduza
#include <Admin/AdminFrame.hpp>
#include <Noder/NoderFrame.hpp>
#include <Widgets/PzaVTabWidget.hpp>
#include <StoreBar/StoreBar.hpp>


/**
 *
 * 
 */
class TabSystem : public QWidget
{
    public:
        TabSystem(QWidget *parent = nullptr)
            : QWidget(parent)
        {

            connect(&Store::Get().connection.admin, &AdminConnection::newSystemMessageReady, [this](){
                
                SystemMessage msg;
                if( Store::Get().connection.admin.popSystemMessage(msg) )
                {
                    switch(msg.stream)
                    {
                        case SystemStreamBegCmd:
                            mLogZone.setTextColor(QColor("#f39c12")); break;
                        case SystemStreamEndCmd:
                            mLogZone.setTextColor(QColor("#16a085")); break;
                        case SystemStreamStdErr:
                            mLogZone.setTextColor(QColor("#e74c3c")); break;
                        case SystemStreamStdOut:
                            mLogZone.setTextColor(QColor("#FFFFFF")); break;
                    }
                    mLogZone.append( QString(msg.data) );
                }
            } );
            



            auto b22 = new QPushButton("install ssh server");
            connect(b22, &QPushButton::clicked, [](){
                Store::Get().connection.admin.execInstallSshServer();
            });


            auto b33 = new QPushButton("uninstall ssh server");
            connect(b33, &QPushButton::clicked, [](){
                Store::Get().connection.admin.execUninstallSshServer();
            });


            //  
            auto lay = new QVBoxLayout(this);
            
            lay->addWidget(b22);
            lay->addWidget(b33);
            lay->addWidget(&mLogZone);


        }
        
    private:

        QTextEdit mLogZone;

};

