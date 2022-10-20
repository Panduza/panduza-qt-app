#include "PzaDoubleSpinBox.hpp"

PzaDoubleSpinBox::PzaDoubleSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent)
{
    setStyleSheet(
        "QDoubleSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #191919;"
        "}"
        "QDoubleSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #282828;"
        "}"
    );
}