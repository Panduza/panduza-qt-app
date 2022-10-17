#include "PzaWidget.hpp"

PzaWidget::PzaWidget(QWidget *parent)
    : QWidget(parent)
{

}

void PzaWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        clicked();
    }
}