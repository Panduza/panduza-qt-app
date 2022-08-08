#ifndef GROUP_HPP
#define GROUP_HPP

#include <PzaUtils.hpp>
#include <Interface.hpp>

class Machine;

class Group
{
    public:
        Group(Machine *machine, const QString &name);
        ~Group() = default;

        Interface *findInterface(const QString &name)
        {
            return (PzaUtils::isInHash<Interface *>(_interfaceMap, name)) ?_interfaceMap[name] : nullptr;
        }

        void addInterface(Interface *interface)
        {
            _interfaceMap[interface->name()] = interface;
        }

        const QString &name() const {return _name;}
        Machine *machine() const {return _machine;}
        const std::unordered_map<QString, Interface *> &interfaces(void) const {return _interfaceMap;}

    private:
        QString _name;
        Machine *_machine;
        std::unordered_map<QString, Interface *> _interfaceMap;
};

#endif