#include <GNodeOp.hpp>

GNodeOp::GNodeOp(const QString &name)
    : GNodeBasic(name)
{
    setType(NodeProperty::Type::Operation);
}