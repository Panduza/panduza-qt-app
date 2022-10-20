#pragma once

#include <QMenuBar>
#include <PzaWidget.hpp>

class PzaMenuBar : public QMenuBar
{
    public:
        PzaMenuBar(QWidget *parent = nullptr);
        ~PzaMenuBar() = default;
};