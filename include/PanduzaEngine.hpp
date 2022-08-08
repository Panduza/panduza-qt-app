#ifndef PANDUZAENGINE_HPP
#define PANDUZAENGINE_HPP

#include <QObject>
#include <Broker.hpp>
#include <Interface.hpp>
#include <Machine.hpp>


typedef std::function<Interface *(Group *group, const QString &name)> t_createInterface;

class PanduzaEngine : public QObject
{
    Q_OBJECT
   
    public:
        PanduzaEngine();
        ~PanduzaEngine();

        const QString &getInterfaceTypeName(const e_InterfaceType type)
        {
            return _sInterfaceTypeMap[type];
        }

        const e_InterfaceType &getInterfaceTypEnum(const QString &type)
        {
            return _eInterfaceTypeMap[type];
        }

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

        std::unordered_map<QString, const e_InterfaceType> _eInterfaceTypeMap = {
            {"platform", E_TYPE_PLATFORM},
            {"io", E_TYPE_IO},
            {"psu", E_TYPE_PSU}
        };
        std::unordered_map<e_InterfaceType, const QString> _sInterfaceTypeMap = {
            {E_TYPE_PLATFORM, "platform"},
            {E_TYPE_IO, "io"},
            {E_TYPE_PSU, "psu"}
        };

        Machine *findMachine(const QString &name);
        Machine *createMachine(const QString &name);

        bool isValidInterfaceType(const QString &name)
        {
            return PzaUtils::isInHash<t_createInterface>(_interfaceTypeMap, name);
        }

    public slots:
        void parseMsg(const QMqttTopicName &topic, const QByteArray &message);

    signals:
        void interfaceRegistered(Interface *);
};

#endif