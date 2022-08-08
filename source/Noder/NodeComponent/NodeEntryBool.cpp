#include <NodeEntryBool.hpp>

NodeEntryBool::NodeEntryBool(QString name, enum NodeEntryDirection dir, Node *parent)
    : NodeEntryVariable(name, dir, parent)
{
    setType(Bool);
    setPlugColor("#E20909");
    
    if (direction() != Output) {
        _checkbox = addWidget<QCheckBox>(0, 1);
        _checkbox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        _layout->setColumnStretch(1, 1);
    }
}

NodeEntryBool::~NodeEntryBool()
{

};