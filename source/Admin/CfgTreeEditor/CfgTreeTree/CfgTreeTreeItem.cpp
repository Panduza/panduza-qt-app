#include "CfgTreeTreeItem.hpp"


// ============================================================================
// 
CfgTreeTreeItem::CfgTreeTreeItem(ItemType type, CfgTreeTreeItem *parentItem)
    : _type(type)
    , _ptrBroker(nullptr)
    , _ptrInterface(nullptr)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
CfgTreeTreeItem::CfgTreeTreeItem(const CfgBroker::ShPtr& data, CfgTreeTreeItem *parentItem)
    : _type(TypeBroker)
    , _ptrBroker(data)
    , _ptrInterface(nullptr)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
CfgTreeTreeItem::CfgTreeTreeItem(const CfgInterface::ShPtr& data, CfgTreeTreeItem *parentItem)
    : _type(TypeInterface)
    , _ptrBroker(nullptr)
    , _ptrInterface(data)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
CfgTreeTreeItem::~CfgTreeTreeItem()
{
    // qDeleteAll(_childItems);
}

// ============================================================================
// 
void CfgTreeTreeItem::appendChild(CfgTreeTreeItem *item)
{
    item->_parentItem = this;
    _childItems.append(item);
}
//! [2]

//! [3]
CfgTreeTreeItem *CfgTreeTreeItem::child(int row)
{
    if (row < 0 || row >= _childItems.size())
        return nullptr;
    return _childItems.at(row);
}

// ============================================================================
// 
int CfgTreeTreeItem::childCount() const
{
    return _childItems.count();
}

// ============================================================================
// 
int CfgTreeTreeItem::columnCount() const
{
    switch (_type)
    {
        case TypeRoot: return 1;
        case TypeBroker: return 2;
        case TypeInterface: return 1;
        default:
            return 0;
    }
}

// ============================================================================
// 
QVariant CfgTreeTreeItem::data(int column) const
{
    switch (_type)
    {
    case TypeRoot:
        break;

    case TypeBroker:
        if(column == 0) return QVariant(_ptrBroker->name());
        if(column == 1) return QVariant();
        break;
    
    case TypeInterface:
        if(column == 0) return QVariant(_ptrInterface->name());
        if(column == 1) return QVariant(_ptrInterface->driver());
        break;
    
    default:
        break;
    }
    return QVariant("not implement");
}

// ============================================================================
// 
bool CfgTreeTreeItem::setData(int column, const QVariant &value)
{
    if(value.canConvert<QString>())
    {
        switch (_type)
        {
        case TypeRoot:
            break;

        case TypeBroker:
            if(column == 0) _ptrBroker->setName(value.toString());
            break;
        
        case TypeInterface:
            if(column == 0) _ptrInterface->setName(value.toString());
            break;
        
        default:
            break;
        }
    }

    return false;
}

// ============================================================================
// 
CfgTreeTreeItem *CfgTreeTreeItem::parentItem()
{
    return _parentItem;
}

// ============================================================================
// 
int CfgTreeTreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<CfgTreeTreeItem*>(this));

    return 0;
}
