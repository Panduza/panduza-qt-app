#include <QJsonDocument>
#include <QJsonObject>

#include <PanduzaEngine.hpp>
#include <InterfaceFamilies.hpp>
#include <PzaTopic.hpp>
#include <PzaUtils.hpp>

PanduzaEngine::PanduzaEngine()
{
    _broker = new Broker();
    connect(_broker, &Broker::msgReceived, this, &PanduzaEngine::parseMsg);

    _interfaceTypeMap["platform"] = PanduzaEngine::createInterface<Platform>;
    _interfaceTypeMap["io"] = PanduzaEngine::createInterface<Io>;
}

Machine *PanduzaEngine::findMachine(const QString &name)
{
    return (PzaUtils::isInHash<Machine *>(_machineMap, name)) ?_machineMap[name] : nullptr;
}

Machine *PanduzaEngine::createMachine(const QString &name)
{
    Machine *machine;

    machine = new Machine(this, name);
    _machineMap[name] = machine;
    return machine;
}

void PanduzaEngine::parseMsg(const QMqttTopicName &topic, const QByteArray &message)
{
    PzaTopic pzaTopic;
    Machine *machine;
    Group *group;
    Interface *interface;
    QString interfaceType;
    QJsonDocument doc;

    if (pzaTopic.parseTopic(topic.name()) == -1)
        return ;

    machine = findMachine(pzaTopic.machine());
    if (!machine) {
        machine = createMachine(pzaTopic.machine());
        group = machine->createGroup(pzaTopic.group());
    }
    else {
        group = machine->findGroup(pzaTopic.group());
        if (!group)
            group = machine->createGroup(pzaTopic.group());
    }

    doc = QJsonDocument::fromJson(message);

    interface = group->findInterface(pzaTopic.interface());
    if (pzaTopic.type() == "info") {
        if (interface)
            return ;

        interfaceType = PzaJSON::keyToString(doc, "type");

        if (isValidInterfaceType(interfaceType)) {
            interface = _interfaceTypeMap[interfaceType](group, pzaTopic.interface());
            _broker->subscribe(interface->path() + "/atts/#");
        }
        else
             qDebug() << "interface type" << interfaceType << "not recognized";
    }
    else if (interface) {
        interface->parseMsg(pzaTopic.property(), doc);

    }
}

void PanduzaEngine::sendMsg(const QString &topic, const QByteArray &message)
{
    _broker->publish(topic, message);
}

void PanduzaEngine::showTree(void)
{
    for (auto machine: machines()) {
        qDebug() << "Machine:" << machine.first;
        for (auto group: machine.second->groups()) {
            qDebug() << "  Group:" << group.second->name();
            for (auto interface: group.second->interfaces()) {
                qDebug() << "    Interface:" << interface.second->name();
            }
        }
    }
}

Interface *PanduzaEngine::findInterface(const QString &machine, const QString &group, const QString &name)
{
    Machine *machinePtr;
    Group *groupPtr;

    machinePtr = findMachine(machine);
    if (!machinePtr)
        return nullptr;

    groupPtr = machinePtr->findGroup(group);
    if (!groupPtr)
        return nullptr;

    return groupPtr->findInterface(name);
}

PanduzaEngine::~PanduzaEngine()
{

}