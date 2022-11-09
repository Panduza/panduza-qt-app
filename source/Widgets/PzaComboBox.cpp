#include <PzaComboBox.hpp>

PzaComboBox::PzaComboBox(QWidget *parent)
    : QComboBox(parent)
{
   
    view()->parentWidget()->setStyleSheet("background-color: #474747");
}

void PzaComboBox::showPopup(void)
{
    QComboBox::showPopup();
    clicked();
}