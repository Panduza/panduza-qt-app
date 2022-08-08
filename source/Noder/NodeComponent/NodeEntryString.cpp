#include <NodeEntryString.hpp>

NodeEntryString::NodeEntryString(QString name, enum NodeEntryDirection dir, Node *parent)
    : NodeEntryVariable(name, dir, parent),
    _txtbox(nullptr)
{
    setType(String);
    setPlugColor("#C05DC2");

    if (direction() != Output) {
        _txtbox = addWidget<QLineEdit>(0, 1);
        _txtbox->setFixedWidth(100);
    }

}

NodeEntryString::~NodeEntryString()
{
    
}