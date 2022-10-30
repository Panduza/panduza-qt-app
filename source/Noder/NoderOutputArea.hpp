#pragma once

#include <PzaTabWidget.hpp>
#include "NoderConsole.hpp"

class NoderOutputArea : public PzaTabWidget
{
    public:
        NoderOutputArea(QWidget *parent = nullptr);

    private:
        NoderConsole *_console;
};