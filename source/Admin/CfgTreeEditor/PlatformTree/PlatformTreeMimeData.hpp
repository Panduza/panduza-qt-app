#pragma once

// Qt
#include <QList>
#include <QVariant>
#include <QMimeData>

// Panduza
#include <Store/workspace/etcpza/platform/Platform.hpp>


class PlatformTreeItem;

class PlatformTreeMimeData : public QMimeData
{
    Q_OBJECT
    
public:

    PlatformTreeMimeData();

    const QList<PlatformTreeItem*>& platformTreeItems() const { return _childItems; }

    void appendPlatformTreeItem(PlatformTreeItem *child);

private:

    /**
     * @brief 
     */
    QList<PlatformTreeItem*> _childItems;

};


