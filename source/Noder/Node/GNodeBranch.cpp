#include <GNodeBranch.hpp>

GNodeBranch::GNodeBranch(const QString &name)
    : GNodeExec(name, 1, 0)
{
    setType(NodeProperty::Type::Branch);
}