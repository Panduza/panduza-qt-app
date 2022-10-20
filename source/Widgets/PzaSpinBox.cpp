#include "PzaSpinBox.hpp"

PzaSpinBox::PzaSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setStyleSheet(
        "QSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #191919;"
        "}"
        "QSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #282828;"
        "}"
    );
}