#include <GNodeOp.hpp>

GNodeOp::GNodeOp(const QString &name)
    : GNode(name)
{
    _type = NodeProperty::Type::Operation;

    setup();
}