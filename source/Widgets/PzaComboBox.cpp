#include <PzaComboBox.hpp>

PzaComboBox::PzaComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setStyleSheet(
        "QComboBox {"
        "   border: 1px solid gray;"
        "   border-radius: 2px;"
        "   font: 14px;"
        "   color: #DCDCDC;"
        "   background-color: #191919;"
        "   padding: 3px 30px 1px 3px;"
        "}"
        "QComboBox::drop-down {"
            "subcontrol-origin: padding;"
            "subcontrol-position: top right;"
            "border-top-right-radius: 3px;"
            "border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QListView {"
        "   background-color: #474747;"
        "}"
    );
    view()->parentWidget()->setStyleSheet("background-color: #474747");
}

void PzaComboBox::showPopup(void)
{
    QComboBox::showPopup();
    clicked();
}