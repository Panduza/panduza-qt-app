#include <GNodeEvent.hpp>

GNodeEvent::GNodeEvent(const QString &name)
    : GNodeExec(name, 0, 1)
{
    setType(NodeProperty::Type::Event);
}