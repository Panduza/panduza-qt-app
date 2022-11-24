// Panduza
#include "ItemEditorView.hpp"


#include "IeBroker.hpp"
#include "IeInterface.hpp"

// ============================================================================
// 
ItemEditorView::ItemEditorView(QWidget *parent)
    : QWidget(parent)
    , _layout(this)
    , _internal(nullptr)
{

}

// ============================================================================
// 
void ItemEditorView::loadItem(const CfgBroker::ShPtr& broker_item)
{
    if(_internal){
        _internal->deleteLater();
    }

    _internal = new IeBroker(broker_item);

    _layout.addWidget(_internal);
}

// ============================================================================
// 
void ItemEditorView::loadItem(const CfgInterface::ShPtr& interface_item)
{

}
