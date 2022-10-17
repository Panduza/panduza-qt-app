#include "PzaLabel.hpp"

PzaLabel::PzaLabel(const QString &name, QWidget *parent)
    : QLabel(name, parent)
{
    setStyleSheet(
        "QLabel {"
        "   color: #DCDCDC;"
        "   background-color: transparent;"
        "   font: 14px;"
        "}"
    );
}

PzaLabel::PzaLabel(QWidget *parent)
    : PzaLabel("", parent)
{

}