// Panduza
#include "TabSystem.hpp"

// ============================================================================
//
TabSystem::TabSystem(QWidget *parent)
    : QWidget(parent)
    , _actionBt("install")
    , _installSshServerRb("ssh server")
{

    // button install settings
    // button install/uninstall -> depend of the check install


    // connect(&Store::Get().connection.admin, &AdminConnection::newSystemMessageReady, [this](){
        
    //     SystemMessage msg;
    //     if( Store::Get().connection.admin.popSystemMessage(msg) )
    //     {
    //         switch(msg.stream)
    //         {
    //             case SystemStreamBegCmd:
    //                 mLogZone.setTextColor(QColor("#f39c12")); break;
    //             case SystemStreamEndCmd:
    //                 mLogZone.setTextColor(QColor("#16a085")); break;
    //             case SystemStreamStdErr:
    //                 mLogZone.setTextColor(QColor("#e74c3c")); break;
    //             case SystemStreamStdOut:
    //                 mLogZone.setTextColor(QColor("#FFFFFF")); break;
    //         }
    //         mLogZone.append( QString(msg.data) );
    //     }
    // } );
    

    // auto b22 = new QPushButton("install ssh server");
    // connect(b22, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execSshServerInstall();
    // });

    // auto b33 = new QPushButton("uninstall ssh server");
    // connect(b33, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execSshServerUninstall();
    // });

    
    // auto bdock = new QPushButton("install docker");
    // connect(bdock, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execInstallDocker();
    // });

    

    // auto b4 = new QPushButton("start platform");
    // connect(b4, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execStartPlatform();
    // });

    // auto b5 = new QPushButton("stop platform");
    // connect(b5, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execStopPlatform();
    // });

    // auto b6 = new QPushButton("start autodetect");
    // connect(b6, &QPushButton::clicked, [](){
    //     Store::Get().connection.admin.execAutodetectPlatform();
    // });

    // //  
    // auto lay = new QVBoxLayout(this);
    
    // lay->addWidget(b22);
    // lay->addWidget(b33);
    // lay->addWidget(bdock);
    // lay->addWidget(b4);
    // lay->addWidget(b5);
    // lay->addWidget(b6);
    // lay->addWidget(&mLogZone);



    connect(&_actionBt, &QPushButton::clicked, this, &TabSystem::systemAction);


    auto side_bar = new QWidget();
    auto side_bar_lay = new QVBoxLayout(side_bar);
    side_bar_lay->addWidget(&_actionBt);
    side_bar_lay->addWidget(&_installSshServerRb);
    


    _splitter.addWidget(side_bar);
    _splitter.addWidget(&_logZone);
    _splitter.setStretchFactor(0, 0);
    _splitter.setStretchFactor(1, 1);



    auto main_lay = new QVBoxLayout(this);
    main_lay->addWidget(&_splitter);

}

// ============================================================================
// 
void TabSystem::systemAction()
{
    
}

// ============================================================================
// 
void TabSystem::checkSystem()
{
    if(_installSshServerRb.isChecked())
    {

    }


}

// ============================================================================
// 
void TabSystem::installSystem()
{

}

// ============================================================================
// 
void TabSystem::uninstallSystem()
{

}

