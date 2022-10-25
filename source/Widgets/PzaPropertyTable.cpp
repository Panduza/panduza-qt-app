#include "PzaPropertyTable.hpp"

PzaPropertyTable::PzaPropertyTable(QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QGridLayout(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _layout->setContentsMargins(3, 3, 3, 3);
}