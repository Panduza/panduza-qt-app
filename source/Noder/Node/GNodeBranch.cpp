#include <GNodeBranch.hpp>

GNodeBranch::GNodeBranch(const QString &name)
    : GNodeExec(name, 1, 0)
{
    _type = NodeProperty::Type::Branch;

    setup();
}