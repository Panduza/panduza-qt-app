#include "PzaLabel.hpp"

PzaLabel::PzaLabel(const QString &name, QWidget *parent)
    : QLabel(name, parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumWidth(200);
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