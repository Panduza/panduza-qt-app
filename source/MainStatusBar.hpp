#pragma once

// Qt
#include <QWidget>
#include <QSplitter>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QPushButton>

// Panduza
#include <Store/Store.hpp>
#include <Noder/NoderFrame.hpp>
#include <Widgets/PzaLabel.hpp>
#include <Widgets/PzaVTabWidget.hpp>
#include <StoreBar/StoreBar.hpp>


/**
 * @brief Display information at the bottom of the main window
 * 
 */
class MainStatusBar : public QStatusBar
{
    public:
        MainStatusBar(QWidget *parent = nullptr)
            : QStatusBar(parent)
            , _adminConnectionStatus("Admin")
        {

            // auto ssh_widget = new QPushButton("SSH");
            // ssh_widget->setToolTip("test");

            // insertWidget(0, new PzaLabel("NOKK"));
            // insertWidget(0, &_adminConnectionStatus);
            // insertWidget(0, new PzaLabel("NOKK"));
            // insertWidget(0, new PzaLabel("MQTT"));


            // connect( &Store::Get().connection.ssh, &SshConnection::statusChanged, [this](){
            //         if(Store::Get().connection.ssh.isReady())
            //         {
            //             _adminConnectionStatus.setText("SSH OK !");
            //         }
            //         else
            //         {
            //             _adminConnectionStatus.setText("KO SSH KO");
            //         }
            //         _adminConnectionStatus.setToolTip(Store::Get().connection.ssh.errString());
            //     });
        }

    private:


        QPushButton _adminConnectionStatus;



};

