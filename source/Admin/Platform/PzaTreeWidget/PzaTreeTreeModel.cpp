

#include "PzaTreeTreeItem.hpp"
#include "PzaTreeTreeModel.hpp"

#include <QDebug>

#include <QStringList>

//! [0]
PzaTreeTreeModel::PzaTreeTreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new PzaTreeTreeItem({tr("Name"), tr("Driver")});
    setupModelData(data.split('\n'), rootItem);
}
//! [0]

//! [1]
PzaTreeTreeModel::~PzaTreeTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int PzaTreeTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<PzaTreeTreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant PzaTreeTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    PzaTreeTreeItem *item = static_cast<PzaTreeTreeItem*>(index.internalPointer());


    // qDebug() << index << " === " << item->data(index.column());

    return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags PzaTreeTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant PzaTreeTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex PzaTreeTreeModel::index(int row, int column, const QModelIndex &parent) const
{


    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PzaTreeTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PzaTreeTreeItem*>(parent.internalPointer());

    PzaTreeTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}
//! [6]
//! [7]
QModelIndex PzaTreeTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    PzaTreeTreeItem *childItem = static_cast<PzaTreeTreeItem*>(index.internalPointer());
    PzaTreeTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int PzaTreeTreeModel::rowCount(const QModelIndex &parent) const
{
    PzaTreeTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PzaTreeTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]

void PzaTreeTreeModel::setupModelData(const QStringList &lines, PzaTreeTreeItem *parent)
{

    QList<QVariant> c1;
    c1 << "first";
    auto i1 = new PzaTreeTreeItem(c1, rootItem);
    rootItem->appendChild(i1);


    QList<QVariant> c2;
    c2 << "second" << "lol";
    auto i2 = new PzaTreeTreeItem(c2, rootItem);
    rootItem->appendChild(i2);


    QList<QVariant> c3;
    c3 << "pok" << "plllala";
    auto i3 = new PzaTreeTreeItem(c3, i2);
    i2->appendChild(i3);


    // QList<PzaTreeTreeItem *> parents;
    // QList<int> indentations;
    // parents << parent;
    // indentations << 0;

    // int number = 0;

    // while (number < lines.count()) {
    //     int position = 0;
    //     while (position < lines[number].length()) {
    //         if (lines[number].at(position) != ' ')
    //             break;
    //         position++;
    //     }

    //     const QString lineData = lines[number].mid(position).trimmed();

    //     if (!lineData.isEmpty()) {
    //         // Read the column data from the rest of the line.
    //         const QStringList columnStrings =
    //             lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
    //         QList<QVariant> columnData;
    //         columnData.reserve(columnStrings.count());
    //         for (const QString &columnString : columnStrings)
    //             columnData << columnString;

    //         if (position > indentations.last()) {
    //             // The last child of the current parent is now the new parent
    //             // unless the current parent has no children.

    //             if (parents.last()->childCount() > 0) {
    //                 parents << parents.last()->child(parents.last()->childCount()-1);
    //                 indentations << position;
    //             }
    //         } else {
    //             while (position < indentations.last() && parents.count() > 0) {
    //                 parents.pop_back();
    //                 indentations.pop_back();
    //             }
    //         }

    //         // Append a new item to the current parent's list of children.
    //         parents.last()->appendChild(new PzaTreeTreeItem(columnData, parents.last()));
    //     }
    //     ++number;
    // }
}
