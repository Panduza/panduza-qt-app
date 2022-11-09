#include "PzaTreeWidgetItem.hpp"

PzaTreeWidgetItem::PzaTreeWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
{

}

PzaTreeWidgetItem::PzaTreeWidgetItem(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{

}

void PzaTreeWidgetItem::setSvgIcon(int index, const QByteArray &data)
{
    QPixmap pixmap;

    pixmap.loadFromData(data);
    QTreeWidgetItem::setIcon(index, pixmap);
}