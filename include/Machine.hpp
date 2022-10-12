#pragma once

#include <QString>

#include <Group.hpp>

class PanduzaEngine;

class Machine
{
    public:
        Machine(PanduzaEngine *engine, const QString &name);
        ~Machine() = default;

        Group *findGroup(const QString &name)
        {
            return (PzaUtils::isInStdMap<Group *>(_groupMap, name)) ?_groupMap[name] : nullptr;
        }

        Group *createGroup(const QString &name);

        const QString &name(void) const {return _name;}
        PanduzaEngine *engine(void) const {return _engine;}
        std::unordered_map<QString, Group *> &groups(void) {return _groupMap;}

    private:
        PanduzaEngine *_engine;
        QString _name;
        std::unordered_map<QString, Group *> _groupMap;
};