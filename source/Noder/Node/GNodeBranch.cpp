#include <GNodeBranch.hpp>

GNodeBranch::GNodeBranch(const QString &name)
    : GNodeExec(name, NodeExecDirection::Input)
{
    setType(NodeProperty::Type::Branch);
}