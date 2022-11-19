#pragma once

// Qt
#include <QList>
#include <QVariant>

// Panduza
#include <Store/workspace/etcpza/platform/Platform.hpp>


class PlatformTreeItem
{
public:

    enum ItemType {
        TypeNone, TypeRoot, TypeDriver, TypeAutoDetect
    };


    PlatformTreeItem(ItemType type = TypeNone, PlatformTreeItem *parentItem = nullptr);
    PlatformTreeItem(const DriverInfo::ShPtr& data, PlatformTreeItem *parentItem = nullptr);
    PlatformTreeItem(const AutoDetectInfo::ShPtr& data, PlatformTreeItem *parentItem = nullptr);

    ~PlatformTreeItem();

    void appendChild(PlatformTreeItem *child);

    PlatformTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PlatformTreeItem *parentItem();

    ItemType type() const { return _type; }


    /**
     * @brief Getter for the internal driver info pointer
     */
    const DriverInfo::ShPtr& ptrDriverInfo() const { return _ptrDriverInfo; }

    /**
     * @brief Getter for the internal interface auto detect pointer
     */
    const AutoDetectInfo::ShPtr& ptrAutoDetectInfo() const { return _ptrAutoDetectInfo; }


    QJsonObject toInterfaceJson() const;


private:

    ItemType _type;

    DriverInfo::ShPtr _ptrDriverInfo;
    AutoDetectInfo::ShPtr _ptrAutoDetectInfo;

    /**
     * @brief 
     */
    PlatformTreeItem *_parentItem;

    /**
     * @brief 
     */
    QList<PlatformTreeItem*> _childItems;
};


