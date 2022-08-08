#include <NodeEntryInteger.hpp>

NodeEntryInteger::NodeEntryInteger(QString name, enum NodeEntryDirection dir, Node *parent)
    : NodeEntryVariable(name, dir, parent),
    _valuebox(nullptr)
{
    setType(Integer);
    setPlugColor("#19C5CC");

    if (direction() != Output) {
        _valuebox = addWidget<QSpinBox>(0, 1);
        _valuebox->setMinimum(std::numeric_limits<int32_t>::min());
        _valuebox->setMaximum(std::numeric_limits<int32_t>::max());
    }
}

NodeEntryInteger::~NodeEntryInteger()
{
    
}