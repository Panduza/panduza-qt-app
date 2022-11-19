// Panduza
#include "PlatformTreeItem.hpp"
#include "PlatformTreeModel.hpp"
#include <Store/Store.hpp>
#include "PlatformTreeMimeData.hpp"

// Qt
#include <QDebug>
#include <QMimeData>
#include <QStringList>

// ============================================================================
//
PlatformTreeModel::PlatformTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    connect(&Store::Get().workspace.etc.platform, &Platform::updated, this, &PlatformTreeModel::syncWithStore);
    syncWithStore();        
}

// ============================================================================
//
PlatformTreeModel::~PlatformTreeModel()
{
    
}

// ============================================================================
//
int PlatformTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<PlatformTreeItem*>(parent.internalPointer())->columnCount();
    return _rootItem->columnCount();
}

// ============================================================================
//
QVariant PlatformTreeModel::data(const QModelIndex &index, int role) const
{
    // Debug
    // qDebug() << "model::data" << index << " / " << role ;


    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    PlatformTreeItem *item = static_cast<PlatformTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

// ============================================================================
//
Qt::ItemFlags PlatformTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;
}

// ============================================================================
//
QVariant PlatformTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    // Debug
    // qDebug() << "model::headerData" << section << " / " << orientation << " / " << role;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant("top");


    return QVariant();
}

// ============================================================================
//
QModelIndex PlatformTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // Debug
    // qDebug() << "model::index " << row << " / " << column << " / " << parent;

    // Index invalid
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // Get the object at the given from the parent item
    // rows are relative to a given parent
    PlatformTreeItem *parentItem;
    if (!parent.isValid()) { parentItem = _rootItem; }
    else { parentItem = static_cast<PlatformTreeItem*>(parent.internalPointer()); }
    PlatformTreeItem *childItem = parentItem->child(row);
    if (!childItem)
        return QModelIndex();

    // Associate the item to the index
    return createIndex(row, column, childItem);
}


QModelIndex PlatformTreeModel::parent(const QModelIndex &index) const
{
    // Debug
    // qDebug() << "model::parent " << index;

    if (!index.isValid())
        return QModelIndex();

    PlatformTreeItem *childItem = static_cast<PlatformTreeItem*>(index.internalPointer());
    PlatformTreeItem *parentItem = childItem->parentItem();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


// ============================================================================
// 
int PlatformTreeModel::rowCount(const QModelIndex &parent) const
{
    PlatformTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<PlatformTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

// ============================================================================
// 
QMimeData* PlatformTreeModel::mimeData(const QModelIndexList &indexes) const
{
    // qDebug() << "PlatformTreeModel::mimeData";
    // for(auto idx : indexes)
    // {
    //     qDebug() << " - " << idx;
    // }


    auto mimed = new PlatformTreeMimeData();
    for(auto idx : indexes)
    {
        auto item = static_cast<PlatformTreeItem*>(idx.internalPointer());
        mimed->appendPlatformTreeItem(item);
    }
    return mimed;
}


QStringList PlatformTreeModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

Qt::DropActions PlatformTreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}


// ============================================================================
// 
void PlatformTreeModel::syncWithStore()
{
    qDebug() << "syncWithStore";
    
    auto driver_infos = Store::Get().workspace.etc.platform.driverInfos();

    // Create the root item
    _rootItem = new PlatformTreeItem(PlatformTreeItem::TypeRoot);
    
    // Go through drivers
    auto info = driver_infos.begin();
    for (; info != driver_infos.end(); ++info)
    {
        // qDebug() << info.key() << ": " << info.value() << Qt::endl;

        _rootItem->appendChild(new PlatformTreeItem(info.value()));
    }

}

