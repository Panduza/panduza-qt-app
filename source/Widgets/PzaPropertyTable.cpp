#include "PzaPropertyTable.hpp"

PzaPropertyTable::PzaPropertyTable(QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _grid = new QGridLayout(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _grid->setContentsMargins(3, 3, 3, 3);
}