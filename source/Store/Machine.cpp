#include "Machine.hpp"

Machine::Machine(const QString &name)
    : _name(name)
{

}

Group *Machine::createGroup(const QString &name)
{
    Group *group;

    group = new Group(this, name);
    _groupMap[name] = group;
    return group;
}