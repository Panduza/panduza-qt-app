#pragma once

#include <PzaTabWidget.hpp>
#include "NoderConsole.hpp"

class NoderOutputArea : public PzaTabWidget
{
    public:
        NoderOutputArea(QWidget *parent = nullptr);
        ~NoderOutputArea() = default;

    private:
        NoderConsole *_console;
};