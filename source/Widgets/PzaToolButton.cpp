#include "PzaToolButton.hpp"

PzaToolButton::PzaToolButton(QWidget *parent)
    : QToolButton(parent)
{

}

PzaToolButton::PzaToolButton(const QString &name, QWidget *parent)
    : QToolButton(parent)
{
    setText(name);
}

void PzaToolButton::setIcon(const QByteArray &data)
{
    QPixmap pixmap;

    pixmap.loadFromData(data);
    QToolButton::setIcon(pixmap);
}