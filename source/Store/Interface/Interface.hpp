#pragma once

#include <PzaJSON.hpp>

class Machine;
class Group;

namespace PzaInterface {

class Interface : public QObject
{
    public:
        const QString &name() const {return _name;}
        const QString &path() const {return _path;}

        void parseMsg(const QString &atts, const QJsonDocument &json);
        void sendMsg(const QString &cmds, const QByteArray &msg);

    protected:
        Interface(Group *group, const QString &name);

        typedef std::function<void(const QJsonDocument &json)> t_fAtts;
        typedef std::function<void(const QJsonDocument &json)> t_fCmds;
        std::unordered_map<QString, t_fAtts> _attsMap;
        std::unordered_map<QString, t_fCmds> _cmdsMap;

        Machine *_machine;
        QString _name;
        Group *_group;
        QString _path;
};

}