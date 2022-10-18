#include "AdminFrame.hpp"

#include <Store/Store.hpp>


// Qt
#include <QDebug>

// Panduza
#include <Store/Store.hpp>
#include "System/TabSystem.hpp"
#include "Platform/TabPlatform.hpp"
#include "Data/TabData.hpp"


// ============================================================================
//
AdminFrame::AdminFrame()
    : QWidget(nullptr)
    , mTabTool(new TabSystem())
    , mTabPlatform(new TabPlatform())
    , mTabData(new TabData())
{

    auto tabContainer = new QWidget();

    auto tabs = new PzaTabWidget();
    tabs->addTab(mTabTool, "System");
    tabs->addTab(mTabPlatform, "Platform");
    tabs->addTab(mTabData, "Data");

    auto main_layout = new QHBoxLayout(tabContainer);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(tabs);


    // a valid workspace is required to boot the application
    // 
    // admin connection ready is required for the admin tab
    // also check for installation packages
    // 


    _content = new QStackedWidget();
    _content->addWidget(new QLabel("Admin Connection must be Valid"));
    _content->addWidget(tabContainer);
    // _content->addWidget(new QLabel("Panudza not installed on your server install it ?"));


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_content);


    // install ssh server
    // install ssh docker
    // install ssh platforms




    // Display admin view only if the admin connection is ready
    // To do so, use the tabwidget to mask the view
    auto display_admin_view_if_admin_conn_ready = [this](){
        if( Store::Get().connection.admin.isReady() )
        {
            _content->setCurrentIndex(1);
        }
        else
        {
            _content->setCurrentIndex(0);
        }
    };
    connect(&Store::Get().connection.admin, &AdminConnection::statusChanged, display_admin_view_if_admin_conn_ready);
    display_admin_view_if_admin_conn_ready();
    

}


