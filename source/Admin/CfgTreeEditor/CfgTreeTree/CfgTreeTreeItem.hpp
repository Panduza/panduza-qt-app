#pragma once

// Qt
#include <QList>
#include <QVariant>

// Panduza
#include <Store/workspace/etcpza/cfgtree/CfgTree.hpp>


class CfgTreeTreeItem
{
public:

    enum ItemType {
        TypeNone, TypeRoot, TypeBroker, TypeInterface
    };


    CfgTreeTreeItem(ItemType type = TypeNone, CfgTreeTreeItem *parentItem = nullptr);
    CfgTreeTreeItem(const CfgBroker::ShPtr& data, CfgTreeTreeItem *parentItem = nullptr);
    CfgTreeTreeItem(const CfgInterface::ShPtr& data, CfgTreeTreeItem *parentItem = nullptr);

    ~CfgTreeTreeItem();


    ItemType type() const { return _type; }

    void appendChild(CfgTreeTreeItem *child);

    /**
     * @brief Inserts a single child before the given row in the child items.
     * 
     * @param child item to insert
     * @param row row before which the item will be inserted
     */
    void insertChild(CfgTreeTreeItem *child, int row = 0);


    CfgTreeTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CfgTreeTreeItem *parentItem();


    bool setData(int column, const QVariant &value);

    /**
     * @brief Getter for the internal broker config pointer
     */
    const CfgBroker::ShPtr& ptrBroker() const { return _ptrBroker; }

    /**
     * @brief Getter for the internal interface config pointer
     */
    const CfgInterface::ShPtr& ptrInterface() const { return _ptrInterface; }

private:

    /**
     * @brief Type of the item
     */
    ItemType _type;

    /**
     * @brief 
     */
    CfgBroker::ShPtr _ptrBroker;

    /**
     * @brief 
     */
    CfgInterface::ShPtr _ptrInterface;
    
    /**
     * @brief 
     */
    CfgTreeTreeItem *_parentItem;

    /**
     * @brief 
     */
    QList<CfgTreeTreeItem*> _childItems;
};


