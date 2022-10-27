#include "NoderOutputArea.hpp"

NoderOutputArea::NoderOutputArea(QWidget *parent)
    : PzaTabWidget(parent)
{
    _console = new NoderConsole(this);

    addTab(_console, "Console");
}