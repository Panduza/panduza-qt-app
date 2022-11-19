// Panduza
#include "ItemEditor.hpp"


#include "IeBroker.hpp"



ItemEditor::ItemEditor(QWidget *parent)
    : QWidget(parent)
    , _layout(this)
    , _internal(nullptr)
{

}


void ItemEditor::loadItem(const CfgBroker::ShPtr& broker_item)
{
    if(_internal){
        _internal->deleteLater();
    }

    _internal = new IeBroker(broker_item);

    _layout.addWidget(_internal);
}
