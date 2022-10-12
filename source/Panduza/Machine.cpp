#include "Machine.hpp"
#include <PanduzaEngine.hpp>

Machine::Machine(PanduzaEngine *engine, const QString &name)
    : _engine(engine),
    _name(name)
{

}

Group *Machine::createGroup(const QString &name)
{
    Group *group;

    group = new Group(this, name);
    _groupMap[name] = group;
    return group;
}