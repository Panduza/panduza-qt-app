#include "Interface.hpp"
#include <PanduzaEngine.hpp>
#include <PzaUtils.hpp>

Interface::Interface(Group *group, const QString &name)
    : _name(name),
    _group(group)
{
    _path = "pza/" + group->machine()->name() + "/" + group->name() + "/" + _name;
    _machine = group->machine();
    _engine = _machine->engine();
    group->addInterface(this);
}

void Interface::parseMsg(const QString &atts, const QJsonDocument &json)
{
    if (PzaUtils::isInStdMap<t_fAtts>(_attsMap, atts))
        _attsMap[atts](json);
}

void Interface::sendMsg(const QString &cmd, const QByteArray &msg)
{
    _engine->sendMsg(path() + "/cmds/" + cmd + "/set", msg);
}