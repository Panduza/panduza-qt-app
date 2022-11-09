#include "PzaPropertyTable.hpp"

PzaPropertyTable::PzaPropertyTable(QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QFormLayout(this);
    _layout->setContentsMargins(3, 3, 3, 3);

    _layout->setAlignment(Qt::AlignVCenter);
}