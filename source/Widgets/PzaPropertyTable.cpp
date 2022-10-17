#include "PzaPropertyTable.hpp"

PzaPropertyTable::PzaPropertyTable(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QGridLayout(this);
}