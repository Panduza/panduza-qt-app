#pragma once

#include <QObject>
#include <Broker.hpp>
#include <Interface.hpp>
#include <Machine.hpp>

using namespace PzaInterface;

typedef std::function<Interface *(Group *group, const QString &name)> t_createInterface;

class PanduzaEngine : public QObject
{
    Q_OBJECT
   
    public:
        PanduzaEngine();
        ~PanduzaEngine();

        const std::unordered_map<QString, Machine *> &machines(void) {return _machineMap;}

        template<typename N>
        static Interface *createInterface(Group *group, const QString &name)
        {
            return new N(group, name);
        }

        void sendMsg(const QString &topic, const QByteArray &message);

        Interface *findInterface(const QString &machine, const QString &group, const QString &name);

        void showTree(void);

    private:
        Broker *_broker;
        std::unordered_map<QString, Machine *> _machineMap;
        
        std::unordered_map<QString, t_createInterface> _interfaceTypeMap;

        Machine *findMachine(const QString &name);
        Machine *createMachine(const QString &name);

        bool isValidInterfaceType(const QString &name)
        {
            return PzaUtils::isInStdMap<t_createInterface>(_interfaceTypeMap, name);
        }

    public slots:
        void parseMsg(const QMqttTopicName &topic, const QByteArray &message);

    signals:
        void interfaceRegistered(Interface *);
};