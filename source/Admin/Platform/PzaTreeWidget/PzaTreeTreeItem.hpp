#pragma once

#include <QVariant>
#include <QList>

//! [0]
class PzaTreeTreeItem
{
public:
    explicit PzaTreeTreeItem(const QList<QVariant> &data, PzaTreeTreeItem *parentItem = nullptr);
    ~PzaTreeTreeItem();

    void appendChild(PzaTreeTreeItem *child);

    PzaTreeTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PzaTreeTreeItem *parentItem();

private:
    QList<PzaTreeTreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    PzaTreeTreeItem *m_parentItem;
};
//! [0]

