#include <NodeEntryInterface.hpp>

NodeEntryInterface::NodeEntryInterface(QString name, enum NodeEntryDirection dir, Node *parent)
    : NodeEntryVariable(name, dir, parent)
{
    setType(InterfaceType);
    setPlugColor("#CE6135");
}

NodeEntryInterface::~NodeEntryInterface()
{

}