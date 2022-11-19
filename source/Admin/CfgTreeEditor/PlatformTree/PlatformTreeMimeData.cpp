// Panduza
#include "PlatformTreeMimeData.hpp"

// ============================================================================
// 
PlatformTreeMimeData::PlatformTreeMimeData()
    : QMimeData()
{

}

// ============================================================================
// 
void PlatformTreeMimeData::appendPlatformTreeItem(PlatformTreeItem *item)
{
    _childItems << item;
}

