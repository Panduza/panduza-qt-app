#include <GNodeEvent.hpp>

GNodeEventStart::GNodeEventStart(const QString &name)
    : GNodeExec(name, NodeExecDirection::Output)
{
    setType(NodeProperty::Type::Event);
}

GNodeEventEnd::GNodeEventEnd(const QString &name)
    : GNodeExec(name, NodeExecDirection::Input)
{
    setType(NodeProperty::Type::Event);
}