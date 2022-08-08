#include <NodeEntryDouble.hpp>

NodeEntryDouble::NodeEntryDouble(QString name, enum NodeEntryDirection dir, Node *parent)
    : NodeEntryVariable(name, dir, parent),
    _valuebox(nullptr)
{
    setType(Double);
    setPlugColor("#3AC242");

    if (direction() != Output) {
        _valuebox = addWidget<QDoubleSpinBox>(0, 1);
        _valuebox->setMinimum(std::numeric_limits<int32_t>::min());
        _valuebox->setMaximum(std::numeric_limits<int32_t>::max());
        _valuebox->setFixedWidth(100);
    }
}

NodeEntryDouble::~NodeEntryDouble()
{
    
}