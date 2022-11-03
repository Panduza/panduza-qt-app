#include <GNodeEvent.hpp>

GNodeEvent::GNodeEvent(const QString &name)
    : GNodeExec(name, NodeExecDirection::Bidirectionnal)
{
    setType(NodeProperty::Type::Event);
}

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