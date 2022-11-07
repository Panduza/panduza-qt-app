#pragma once

#include <PzaTabWidget.hpp>
#include "NoderConsole.hpp"

class NoderOutputArea : public PzaTabWidget
{
    friend class NoderGraph;

    private:
        NoderOutputArea(QWidget *parent = nullptr);

        NoderConsole Console;
};