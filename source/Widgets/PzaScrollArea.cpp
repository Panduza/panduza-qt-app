#include "PzaScrollArea.hpp"

PzaScrollArea::PzaScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setWidgetResizable(true);
}