#include "PzaLabel.hpp"

PzaLabel::PzaLabel(const QString &name, QWidget *parent)
    : QLabel(name, parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumWidth(200);
}

void PzaLabel::mousePressEvent(QMouseEvent *event)
{
    // Makes no sense in our case to click on a label.
    // event->ignore() allows to notify the parent about the press event.
    event->ignore();
}

PzaLabel::PzaLabel(QWidget *parent)
    : PzaLabel("", parent)
{

}