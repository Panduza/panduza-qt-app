#include "PzaCheckBox.hpp"

PzaCheckBox::PzaCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    setStyleSheet(
        "QCheckBox::indicator {"
        "   width: 14px;"
        "   height: 14px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   border-image: url(:/images/noder/checkbox/unchecked.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:unchecked:hover {"
        "   border-image: url(:/images/noder/checkbox/hover.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:checked {"
        "   border-image: url(:/images/noder/checkbox/checked.png) 0 0 0 0 stretch;"
        "}"
    );
}