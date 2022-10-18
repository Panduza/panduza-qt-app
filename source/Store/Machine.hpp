#pragma once

#include <QString>

#include <Group.hpp>

class Machine
{
    public:
        Machine(const QString &name);

        Group *findGroup(const QString &name)
        {
            return (PzaUtils::IsInStdMap<Group *>(_groupMap, name)) ?_groupMap[name] : nullptr;
        }

        Group *createGroup(const QString &name);

        const QString &name(void) const {return _name;}
        std::unordered_map<QString, Group *> &groups(void) {return _groupMap;}

    private:
        QString _name;
        std::unordered_map<QString, Group *> _groupMap;
};