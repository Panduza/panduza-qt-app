#pragma once

#include <PzaUtils.hpp>
#include <Interface/Interface.hpp>

class Machine;

using namespace PzaInterface;

class Group
{
    public:
        Group(Machine *machine, const QString &name);
        ~Group() = default;

        Interface *findInterface(const QString &name)
        {
            return (PzaUtils::IsInStdMap<Interface *>(_interfaceMap, name)) ?_interfaceMap[name] : nullptr;
        }

        void addInterface(Interface *interface)
        {
            _interfaceMap[interface->name()] = interface;
        }

        const QString &name() const {return _name;}
        Machine *machine() const {return _machine;}
        const std::unordered_map<QString, Interface *> &interfaces(void) const {return _interfaceMap;}

    private:
        Machine *_machine;
        QString _name;
        std::unordered_map<QString, Interface *> _interfaceMap;
};