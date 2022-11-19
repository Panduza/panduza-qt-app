#include "PlatformTreeItem.hpp"


// ============================================================================
// 
PlatformTreeItem::PlatformTreeItem(ItemType type, PlatformTreeItem *parentItem)
    : _type(type)
    , _ptrDriverInfo(nullptr)
    , _ptrAutoDetectInfo(nullptr)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
PlatformTreeItem::PlatformTreeItem(const DriverInfo::ShPtr& data, PlatformTreeItem *parentItem)
    : _type(TypeDriver)
    , _ptrDriverInfo(data)
    , _ptrAutoDetectInfo(nullptr)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
PlatformTreeItem::PlatformTreeItem(const AutoDetectInfo::ShPtr& data, PlatformTreeItem *parentItem)
    : _type(TypeAutoDetect)
    , _ptrDriverInfo(nullptr)
    , _ptrAutoDetectInfo(data)
    , _parentItem(parentItem)
{ }

// ============================================================================
// 
PlatformTreeItem::~PlatformTreeItem()
{
    // qDeleteAll(_childItems);
}

// ============================================================================
// 
void PlatformTreeItem::appendChild(PlatformTreeItem *item)
{
    item->_parentItem = this;
    _childItems.append(item);
}
//! [2]

//! [3]
PlatformTreeItem *PlatformTreeItem::child(int row)
{
    if (row < 0 || row >= _childItems.size())
        return nullptr;
    return _childItems.at(row);
}
//! [3]

//! [4]
int PlatformTreeItem::childCount() const
{
    return _childItems.count();
}
//! [4]

//! [5]
int PlatformTreeItem::columnCount() const
{
    switch (_type)
    {
        case TypeRoot: return 1;
        case TypeDriver: return 1;
        case TypeAutoDetect: return 1;
        default:
            return 0;
    }
}
//! [5]

//! [6]
QVariant PlatformTreeItem::data(int column) const
{
    switch (_type)
    {
    case TypeRoot:
        break;

    case TypeDriver:
        if(column == 0) return QVariant(_ptrDriverInfo->name);
    

    case TypeAutoDetect:
        break;
    
    default:
        break;
    }
    // if (column < 0 || column >= m_itemData.size())
    //     return QVariant();
    // return m_itemData.at(column);

    return QVariant();
}

// ============================================================================
//
PlatformTreeItem *PlatformTreeItem::parentItem()
{
    return _parentItem;
}

// ============================================================================
//
int PlatformTreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<PlatformTreeItem*>(this));

    return 0;
}

// ============================================================================
//
QJsonObject PlatformTreeItem::toInterfaceJson() const
{
    switch (_type)
    {
    case TypeRoot:
        break;

    case TypeDriver:
        return _ptrDriverInfo->toInterfaceJson();
        
    case TypeAutoDetect:
        return _ptrAutoDetectInfo->toInterfaceJson();
    
    default:
        break;
    }
    return QJsonObject();
}

