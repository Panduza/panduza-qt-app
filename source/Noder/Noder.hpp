#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <csignal>

#include <QString>
#include <QColor>
#include <QVariant>

#include <PzaMenu.hpp>

class Pin;
class GNode;
class NoderFrame;

struct PinProperty
{
    enum class Type
    {
        Value,
        Exec
    };

    enum class Direction
    {
        Input = 0,
        Output
    };
};

struct NodeProperty
{
    enum class Type
    {
        Operation = 0,
        Exec,
        Branch,
        Instance,
        Event
    };

    enum class PlugType {
            Value = 0,
            Array,
            Exec
        };

    struct plugIconData {
        QString filename;
        QString data;
    };

    struct plugIcon {
        struct plugIconData plugC;
        struct plugIconData plugNc;
    };
};

struct NoderVar
{
    enum class Container
    {
        Variable = 0,
        Array
    };

    enum class Type
    {
        Wildcard = 0,
        Int,
        Float,
        Bool,
        String,
        Enum,
        Array,
        Interface
    };
};

struct s_varProperties
{
    QString name;
    QColor color;
    bool canBeInstance;
    bool canBePin;
};

class Noder : public QObject
{
    Q_OBJECT

    public:

        static Noder &Get(void)
        {
            static Noder _database;
            return _database;
        }

        NoderFrame *Frame;
        
        Noder(Noder &other) = delete;
        void operator=(const Noder &) = delete;

        typedef std::function<GNode *(void)> t_CreateNode;

        static QString &pinTypeName(const PinProperty::Type type);
        static PinProperty::Type pinTypeFromName(const QString &name);

        static QString &pinDirName(const PinProperty::Direction dir);
        static PinProperty::Direction pinDirFromName(const QString &name);

        static QString &varTypeName(const NoderVar::Type type);
        static NoderVar::Type varTypeFromName(const QString &name);

        static QString &varContainerName(const NoderVar::Container ctn);
        static NoderVar::Container varContainerFromName(const QString &name);

        static QColor &varTypeColor(const NoderVar::Type type);
        static const s_varProperties &varTypeProperties(const NoderVar::Type type);

        static QString &nodeTypeName(const NodeProperty::Type type);
        static NodeProperty::Type nodeTypeFromName(const QString &name);

        const std::vector<QString> &enumValues(const QString &name);

        void forEachEnum(const std::function<void(const QString &name, const std::vector<QString> &list)> &f);
        void forEachEnumName(const std::function<void(const QString &name)> &f);
        void forEachEnumValues(const QString &name, const std::function<void(const QString &name)> &f);

        static void forEachVarContainer(const std::function<void(const NoderVar::Container ctn)> &f);
        static void forEachVarType(const std::function<void(const NoderVar::Type type)> &f);
        static void forEachPinType(const std::function<void(const PinProperty::Type type)> &f);
        static void forEachPinDirection(const std::function<void(const PinProperty::Direction direction)> &f);

        static const QByteArray &PlugValue(const NoderVar::Type type, bool linked);
        static const QByteArray &PlugArray(const NoderVar::Type type, bool linked);
        static const QByteArray &PlugExec(bool linked);

        static const QByteArray& (*PlugContainerFunc(NoderVar::Container ctn))(NoderVar::Type type, bool linked)
        {
            switch (ctn) {
                case NoderVar::Container::Variable: return PlugValue;
                case NoderVar::Container::Array: return PlugArray;
            };
            return nullptr;
        }

    private:
        Noder();

        static std::unordered_map<NoderVar::Type, s_varProperties> &varTypeMap(void)
        {
            static std::unordered_map<NoderVar::Type, s_varProperties> map = {
                {NoderVar::Type::Bool,      {"Boolean",   "#E20909", true, true}},
                {NoderVar::Type::Int,       {"Integer",   "#19C5CC", true, true}},
                {NoderVar::Type::Wildcard,  {"Wildcard",  "#ffffff", false, false}},
                {NoderVar::Type::Float,     {"Float",     "#3AC242", true, true}},
                {NoderVar::Type::String,    {"String",    "#C05DC2", true, true}},
                {NoderVar::Type::Enum,      {"Enum",      "#CE6135", false, true}},
                {NoderVar::Type::Interface, {"Interface", "#C8B623", false, true}},
                {NoderVar::Type::Array,     {"Array",     "#ffffff", false, true}}
            };
            return map;
        }

        static std::unordered_map<NoderVar::Container, QString> &varContainerMap(void)
        {
            static std::unordered_map<NoderVar::Container, QString> map = {
                {NoderVar::Container::Variable, "Variable"},
                {NoderVar::Container::Array,    "Array"},
            };
            return map;
        }

        static std::unordered_map<NodeProperty::Type, QString> &nodeTypeMap(void)
        {
            static std::unordered_map<NodeProperty::Type, QString> map = {
                {NodeProperty::Type::Operation, "Operation"},
                {NodeProperty::Type::Exec, "Execution"},
                {NodeProperty::Type::Branch, "Branch"},
                {NodeProperty::Type::Instance, "Instance"},
                {NodeProperty::Type::Event , "Event"}
            };
            return map;
        }

        static std::unordered_map<PinProperty::Type, QString> &pinTypeMap(void)
        {
            static std::unordered_map<PinProperty::Type, QString> map = {
                {PinProperty::Type::Exec, "Exec"},
                {PinProperty::Type::Value, "Value"}
            };
            return map;
        }

        static std::unordered_map<PinProperty::Direction, QString> &pinDirMap(void)
        {
            static std::unordered_map<PinProperty::Direction, QString> map = {
                {PinProperty::Direction::Input, "Input"},
                {PinProperty::Direction::Output, "Output"},
            };
            return map;
        }

        static std::map<QString, std::vector<QString>> &enumMap(void)
        {
            static std::map<QString, std::vector<QString>> map = {
                {
                    "I/O direction",
                    {
                        "Input",
                        "Output"
                    }
                },
                {
                    "I/O value",
                    {
                        "Low",
                        "High"
                    }
                }
            };
            return map;
        }

        struct NodeProperty::plugIcon initPlugType(NodeProperty::PlugType type);
        static QByteArray FillPlugMap(const NodeProperty::PlugType &plugType, const NoderVar::Type &type, bool linked);
        static QByteArray LoadCValue(const NoderVar::Type &type);
        static QByteArray LoadNcValue(const NoderVar::Type &type);
        static QByteArray LoadCArray(const NoderVar::Type &type);
        static QByteArray LoadNcArray(const NoderVar::Type &type);

        static inline std::unordered_map<NodeProperty::PlugType, struct NodeProperty::plugIcon> _mapPlugFiles;
        static inline std::unordered_map<NoderVar::Type, QByteArray> _mapPlugValueNcIcon;
        static inline std::unordered_map<NoderVar::Type, QByteArray> _mapPlugValueCIcon;
        static inline std::unordered_map<NoderVar::Type, QByteArray> _mapPlugArrayNcIcon;
        static inline std::unordered_map<NoderVar::Type, QByteArray> _mapPlugArrayCIcon;

        static inline QByteArray _plugExecNcIcon;
        static inline QByteArray _plugExecCIcon;
};