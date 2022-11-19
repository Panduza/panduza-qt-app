#pragma once

// Qt
#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QRadioButton>

// Panduza
#include <Admin/AdminFrame.hpp>
#include <Noder/NoderFrame.hpp>
#include <Widgets/PzaVTabWidget.hpp>
#include <StoreBar/StoreBar.hpp>


/**
 * \brief To manage server system installation
 * 
 */
class TabSystem : public QWidget
{
    public:

        TabSystem(QWidget *parent = nullptr);
    

    private slots:

        void systemAction();

    private:

        /**
         * @brief check if the system is correclty installed
         */
        void checkSystem();

        /**
         * @brief install the system as the user requested
         */
        void installSystem();

        /**
         * @brief uninstall the system as the user requested
         */
        void uninstallSystem();


        // system ok or not ok
        // log console


        QSplitter _splitter;

        QPushButton _actionBt;

        QTextEdit _logZone;



        QRadioButton _installSshServerRb;


};

