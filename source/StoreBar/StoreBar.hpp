#pragma once

// Qt
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>

// Panduza
#include "TabWorkspace.hpp"
#include "TabServer.hpp"

#include <Noder/NoderFrame.hpp>
#include <Widgets/PzaVTabWidget.hpp>


/**
 * @brief Side bar that gather widgets to interact with the store
 * 
 * This side bar is composed of multiple tabs for each aspect of the store
 *
 */
class StoreBar : public QWidget
{
    Q_OBJECT
    
    public:

        StoreBar(QWidget *parent = nullptr)
            : QWidget(parent)
        {

            auto tab_widget = new PzaVTabWidget();
            tab_widget->addTab(new TabServer(), "S");
            tab_widget->addTab(new TabWorkspace(), "W");
            // tab_widget->addTab(new TabInterfaces(), "I");
            
            // Admin
            // Interfaces tree



            // 



            // tab_widget->setTabEnabled( 1, false);
            // I-nterface
            // 

            
            auto layout = new QVBoxLayout(this);
            layout->addWidget(tab_widget);


        }
        

    private:



};

