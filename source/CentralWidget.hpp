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
 * @brief Main widget that defines the main layout
 * 
 */
class CentralWidget : public QWidget
{
    public:
        CentralWidget(QWidget *parent = nullptr)
            : QWidget(parent)
        {


            auto main_layout = new QHBoxLayout(this);

            main_layout->setContentsMargins(0, 0, 0, 0);
            main_layout->addWidget(&mSplitter);

            auto tabs = new PzaTabWidget();
            tabs->addTab(new NoderFrame(nullptr), "Noder");
            tabs->addTab(new AdminFrame(), "Admin");


            auto store_bar = new StoreBar();

            mSplitter.addWidget(tabs);
            mSplitter.addWidget(store_bar);
            mSplitter.setStretchFactor(0, 1);
            mSplitter.setStretchFactor(1, 0);

        }
        


    private:

        QSplitter mSplitter;


};

