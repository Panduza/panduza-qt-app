#include "NoderOutputArea.hpp"

NoderOutputArea::NoderOutputArea(QWidget *parent)
    : PzaTabWidget(parent)
{
    addTab(&Console, "Console");
}