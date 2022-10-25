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
        "QLabel[pzalabel=instance] {"
            "font-style: italic;"
        "}"
    );
}

#include <QColorDialog>

void PzaLabel::mousePressEvent(QMouseEvent *event)
{
    QColorDialog *lol = new QColorDialog();
    lol->exec();
    // Makes no sense in our case to click on a label.
    // event->ignore() allows to notify the parent about the press event.
    event->ignore();
}

PzaLabel::PzaLabel(QWidget *parent)
    : PzaLabel("", parent)
{

}