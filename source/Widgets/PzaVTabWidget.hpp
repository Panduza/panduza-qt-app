#pragma once

// Panduza
#include "PzaVTabBar.hpp"
#include "PzaTabWidget.hpp"


class PzaVTabWidget : public PzaTabWidget
{
    public:
        PzaVTabWidget(QWidget *parent = nullptr)
            : PzaTabWidget(parent)
            {
                setTabBar(new PzaVTabBar);
                setTabPosition(QTabWidget::West);
            }
        ~PzaVTabWidget() = default;
    

};

