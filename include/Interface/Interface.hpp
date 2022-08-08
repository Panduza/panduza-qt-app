#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <PzaJSON.hpp>
#include <InterfaceDataModel.hpp>

class Machine;
class Group;
class PanduzaEngine;

class Interface : public QObject
{
    public:
        const QString &name() const {return _name;}
        const QString &path() const {return _path;}

        virtual const e_InterfaceType type(void) = 0;

        void parseMsg(const QString &atts, const QJsonDocument &json);
        void sendMsg(const QString &cmds, const QByteArray &msg);

    protected:
        Interface(Group *group, const QString &name);
        Interface() = default;
        ~Interface() = default;

        typedef std::function<void(const QJsonDocument &json)> t_fAtts;
        typedef std::function<void(const QJsonDocument &json)> t_fCmds;
        std::unordered_map<QString, t_fAtts> _attsMap;
        std::unordered_map<QString, t_fCmds> _cmdsMap;

        PanduzaEngine *_engine;
        Machine *_machine;
        Group *_group;
        QString _name;
        QString _path;
};

#endif