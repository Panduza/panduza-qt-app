#include "PzaColorBox.hpp"

#include <QTimer>
PzaColorBox::PzaColorBox(QWidget *parent)
    : QLabel(parent)
{
   // _dialog = new QColorDialog();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(30, 20);
    setStyleSheet(
    "   border: 1px solid gray;"
    "   border-radius: 4px;"
    );
}

void PzaColorBox::mousePressEvent(QMouseEvent *event)
{
    QColorDialog *lol = new QColorDialog();
    lol->exec();
}

void PzaColorBox::setColor(const QColor &color)
{
   // qDebug() << _dialog << this;
  //  _dialog->exec();
    _color = color;
    setStyleSheet(
    "   border: 1px solid gray;"
    "   border-radius: 4px;"
    "   background-color: " + color.name()
    );
    //colorChanged();
}