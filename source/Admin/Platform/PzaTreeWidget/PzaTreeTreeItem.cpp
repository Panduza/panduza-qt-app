#include "PzaTreeTreeItem.hpp"

//! [0]
PzaTreeTreeItem::PzaTreeTreeItem(const QList<QVariant> &data, PzaTreeTreeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{}
//! [0]

//! [1]
PzaTreeTreeItem::~PzaTreeTreeItem()
{
    qDeleteAll(m_childItems);
}
//! [1]

//! [2]
void PzaTreeTreeItem::appendChild(PzaTreeTreeItem *item)
{
    m_childItems.append(item);
}
//! [2]

//! [3]
PzaTreeTreeItem *PzaTreeTreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}
//! [3]

//! [4]
int PzaTreeTreeItem::childCount() const
{
    return m_childItems.count();
}
//! [4]

//! [5]
int PzaTreeTreeItem::columnCount() const
{
    return m_itemData.count();
}
//! [5]

//! [6]
QVariant PzaTreeTreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}
//! [6]

//! [7]
PzaTreeTreeItem *PzaTreeTreeItem::parentItem()
{
    return m_parentItem;
}
//! [7]

//! [8]
int PzaTreeTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<PzaTreeTreeItem*>(this));

    return 0;
}
//! [8]

