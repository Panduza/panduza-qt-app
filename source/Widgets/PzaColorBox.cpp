#include "PzaColorBox.hpp"

PzaColorBox::PzaColorBox(QWidget *parent)
    : PzaWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(30, 20);
}

void PzaColorBox::mousePressEvent(QMouseEvent *event)
{
    QColorDialog *dialog;

    dialog = new QColorDialog();
    dialog->setCurrentColor(_color);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &QColorDialog::currentColorChanged, this, &PzaColorBox::setColor);
    connect(dialog, &QColorDialog::colorSelected, this, &PzaColorBox::setColor);
    dialog->exec();
    event->accept();
}

void PzaColorBox::setColor(const QColor &color)
{
    _color = color;
    colorChanged(color);
}