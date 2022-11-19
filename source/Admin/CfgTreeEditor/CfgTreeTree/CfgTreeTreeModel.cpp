// Panduza
#include "CfgTreeTreeItem.hpp"
#include "CfgTreeTreeModel.hpp"
#include <Store/Store.hpp>
#include <PlatformTreeMimeData.hpp>
#include <PlatformTreeItem.hpp>


// Qt
#include <QDebug>
#include <QMimeData>
#include <QStringList>

// ============================================================================
//
CfgTreeTreeModel::CfgTreeTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , _rootItem(nullptr)
    , _lockCount(0)
{
    // unlockExternalSync();

    connect(&Store::Get().workspace.etc.tree, &CfgTree::updatedFromOutside, this, &CfgTreeTreeModel::syncWithStore);
    syncWithStore();
}

// ============================================================================
//
CfgTreeTreeModel::~CfgTreeTreeModel()
{
    
}

// ============================================================================
//
int CfgTreeTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

// ============================================================================
//
QVariant CfgTreeTreeModel::data(const QModelIndex &index, int role) const
{
    // Debug
    // qDebug() << "model::data" << index << " / " << role ;


    if (!index.isValid())
        return QVariant();

    // Returns the same data both in display and edit modes.
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        CfgTreeTreeItem *item = static_cast<CfgTreeTreeItem*>(index.internalPointer());
        return item->data(index.column());
    }

    return QVariant();
}

// ============================================================================
//
bool CfgTreeTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // Set on edit role only
    if (role != Qt::EditRole)
        return false;

    // Get the index item
    CfgTreeTreeItem *item = static_cast<CfgTreeTreeItem*>(index.internalPointer());

    // Modify the data, prevent action from update signal
    bool result = item->setData(index.column(), value);

    // 
    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return result;
}

// ============================================================================
//
Qt::ItemFlags CfgTreeTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;    // Allow drops in the top-level (no parent)

    if (index.column() == 1)
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

// ============================================================================
//
QVariant CfgTreeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Debug
    // qDebug() << "CfgTreeTreeModel::headerData / " << section << " / " << orientation << " / " << role;

    //
    Q_UNUSED(orientation);

    if (role != Qt::DisplayRole)
        return QVariant();

    // 
    switch (section)
    {
        case 0:
            return QVariant(tr("Name"));
        case 1:
            return QVariant(tr("Driver"));
        default:
            break;
    }
    return QVariant();
}

// ============================================================================
//
QModelIndex CfgTreeTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // Debug
    // qDebug() << "model::index " << row << " / " << column << " / " << parent;

    // Index invalid
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    // Get the object at the given from the parent item
    // rows are relative to a given parent
    CfgTreeTreeItem *parentItem;
    if (!parent.isValid()) { parentItem = _rootItem; }
    else { parentItem = static_cast<CfgTreeTreeItem*>(parent.internalPointer()); }
    CfgTreeTreeItem *childItem = parentItem->child(row);
    if (!childItem)
        return QModelIndex();

    // Associate the item to the index
    return createIndex(row, column, childItem);
}

// ============================================================================
//
QModelIndex CfgTreeTreeModel::parent(const QModelIndex &index) const
{
    // Debug
    // qDebug() << "CfgTreeTreeModel::parent / " << index;

    // 
    if (!index.isValid())
        return QModelIndex();

    CfgTreeTreeItem *childItem = static_cast<CfgTreeTreeItem*>(index.internalPointer());

    // if (!childItem)
    //     return QModelIndex();

    CfgTreeTreeItem *parentItem = childItem->parentItem();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

// ============================================================================
// 
int CfgTreeTreeModel::rowCount(const QModelIndex &parent) const
{
    CfgTreeTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<CfgTreeTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

// ============================================================================
// 
QMimeData* CfgTreeTreeModel::mimeData(const QModelIndexList &indexes) const
{
    Q_UNUSED(indexes);

    auto mime = new QMimeData();
    mime->setData("text/txt", "pok");
    return mime;
}

// ============================================================================
// 
bool CfgTreeTreeModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    // Debug
    // qDebug() << "CfgTreeTreeModel::canDropMimeData / " << data->text() << " / " << action << " / " << row << " / " << column << " / " << parent;

    // Refuse drop if the item is not a broker
    auto parentItem = static_cast<CfgTreeTreeItem*>(parent.internalPointer());
    if(parentItem)
    {
        if( parentItem->type() != CfgTreeTreeItem::TypeBroker )
        {
            return false;
        }
    }

    // By default always accept drop
    // By the way if the root item or no parent item refuse drop -> drop checking get stucked
    return true;
}

// ============================================================================
// 
bool CfgTreeTreeModel::dropMimeData(const QMimeData *ext_data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(column);

    // Debug
    // qDebug() << "CfgTreeTreeModel::dropMimeData / " << action << " / " << row << " / " << column << " / " << parent;

    // 
    lockExternalSync();
    auto ext_platform_data = qobject_cast<const PlatformTreeMimeData*>(ext_data);
    if (ext_platform_data)
    {
        for(auto ext : ext_platform_data->platformTreeItems())
        {
            // qDebug() << ext->type();

            if( insertRows(row, 1, parent) )
            {
                auto new_row_index = index( row == -1 ? 0 : row, 0, parent);
                // qDebug() << new_row_index;

                auto new_row_item = static_cast<CfgTreeTreeItem*>(new_row_index.internalPointer());
                // qDebug() << new_row_item->type();
                new_row_item->ptrInterface()->setJsonObject( ext->toInterfaceJson() );


                emit dataChanged(new_row_index, new_row_index, {Qt::DisplayRole, Qt::EditRole});
            }
        }
    }
    unlockExternalSync();
    return true;
}

// ============================================================================
// 
bool CfgTreeTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // Debug
    // qDebug() << "CfgTreeTreeModel::insertRows / " << row << " / " << count << " / " << parent;

    // lock
    lockExternalSync();

    // Get pointer from the parent index
    auto parent_item = static_cast<CfgTreeTreeItem*>(parent.internalPointer());

    // No parent item means that the user is outside any items
    if( !parent_item )
    {
        // Insert new brokers
        beginInsertRows(parent, row, row+count);
        for(int i=0; i<count; i++)
        {
            auto new_broker_obj = Store::Get().workspace.etc.tree.insertBroker();
            auto new_broker_item = new CfgTreeTreeItem( new_broker_obj );
            _rootItem->appendChild(new_broker_item);
        }
        endInsertRows();
    }
    else if( parent_item->type() == CfgTreeTreeItem::TypeBroker )
    {
        // Insert new interfaces
        beginInsertRows(parent, row, row+count);
        for(int i=0; i<count; i++)
        {
            auto new_interface_obj = parent_item->ptrBroker()->insertInterface();
            auto new_interface_item = new CfgTreeTreeItem( new_interface_obj );
            parent_item->appendChild(new_interface_item);
        }
        endInsertRows();
    }

    // Requets view update
    emit layoutChanged();

    // unlock
    unlockExternalSync();
    return true;
}

// ============================================================================
// 
void CfgTreeTreeModel::lockExternalSync()
{
    if(_lockCount == 0)
    {
        disconnect(&Store::Get().workspace.etc.tree, &CfgTree::updated, this, &CfgTreeTreeModel::syncWithStore);
    }
    _lockCount++;
}

// ============================================================================
// 
void CfgTreeTreeModel::unlockExternalSync()
{
    _lockCount--;
    if(_lockCount <= 0)
    {
        _lockCount = 0;
        // connect(&Store::Get().workspace.etc.tree, &CfgTree::updatedFromOutside, this, &CfgTreeTreeModel::syncWithStore);
    }
}

// ============================================================================
// 
void CfgTreeTreeModel::syncWithStore()
{



    emit layoutAboutToBeChanged();

    qDebug() << "tree ::: syncWithStore";
    
 
    if(_rootItem)
    {
        delete _rootItem;
        _rootItem = nullptr;
    }

    // Create the root item
    _rootItem = new CfgTreeTreeItem(CfgTreeTreeItem::TypeRoot);
    



    // Append an item for each broker
    int i = 0;
    auto brokers = Store::Get().workspace.etc.tree.brokers();
    for (const auto& broker : brokers)
    {

        auto broker_item = new CfgTreeTreeItem(broker);
        _rootItem->appendChild(broker_item);


        for (const auto& iface : broker->interfaces())
        {

            auto nnnn = new CfgTreeTreeItem(iface);
            broker_item->appendChild(nnnn);
        }

        i++;
    }



    // emit dataChanged( createIndex(0, 0, _rootItem), createIndex(0, 0, _rootItem)  );




    // void QAbstractItemModel::changePersistentIndex(const QModelIndex &from, const QModelIndex &to)

    // Request Tree View Refresh
    emit layoutChanged();
}

