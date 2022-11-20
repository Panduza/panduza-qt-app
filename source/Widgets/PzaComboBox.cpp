#include <PzaComboBox.hpp>

PzaComboBox::PzaComboBox(QWidget *parent)
    : QComboBox(parent)
{
   
}

void PzaComboBox::showPopup(void)
{
    QComboBox::showPopup();
    clicked();
}