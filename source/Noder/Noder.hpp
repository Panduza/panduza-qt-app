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

#define PIN_PROPERTY_VARIABLE(name) \
    struct PinProperty name = { \
        .type = PinProperty::Type::Variable, \
    };

#define PIN_PROPERTY_EXEC(name) \
    struct PinProperty name = { \
        .type = PinProperty::Type::Exec, \
    };

#define NODER_VAR_REF(name) \
    struct NoderVarProps name = { \
        .container = NoderVarProps::Container::Reference, \
    };

#define NODER_VAR_ARRAY(name) \
    struct NoderVarProps name = { \
        .container = NoderVarProps::Container::Array, \
    };


struct PinProperty
{
    enum class Type
    {
        Variable,
        Exec
    };

    enum class Direction
    {
        Input,
        Output
    };

    Type type;
    Direction direction;
};

struct NodeProperty
{
    enum class Type
    {
        Operation,
        Exec,
        Branch,
        Instance,
        Event
    };

    enum class PlugType {
            Value,
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

struct NoderVarProps
{
    enum class Container
    {
        Reference,
        Array
    };

    enum class Type
    {
        Wildcard,
        Int,
        Float,
        Bool,
        String,
        Enum,
        Interface
    };

    Container container;
    Type type;
    QString subType;
};

struct s_varProperties
{
    QString name;
    QColor color;
    bool canBeInstance;
    bool hasSubType;
    std::vector<NoderVarProps::Type> compatibleMap;
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

        static QString &PinTypeName(const PinProperty::Type type);
        static PinProperty::Type PinTypeFromName(const QString &name);

        static QString &PinDirName(const PinProperty::Direction dir);
        static PinProperty::Direction PinDirFromName(const QString &name);

        static QString &VarTypeName(const NoderVarProps::Type type);
        static NoderVarProps::Type VarTypeFromName(const QString &name);

        static QString &VarContainerName(const NoderVarProps::Container ctn);
        static NoderVarProps::Container VarContainerFromName(const QString &name);

        static QColor &VarTypeColor(const NoderVarProps::Type type);
        static const s_varProperties &VarTypeProperties(const NoderVarProps::Type type);
        static bool VarHasSubType(const NoderVarProps::Type type);


        static QString &NodeTypeName(const NodeProperty::Type type);
        static NodeProperty::Type NodeTypeFromName(const QString &name);

        static const std::vector<QString> &EnumValues(const QString &name);

        static void ForEachEnum(const std::function<void(const QString &name, const std::vector<QString> &list)> &f);
        static void ForEachEnumName(const std::function<void(const QString &name)> &f);
        static void ForEachEnumValues(const QString &name, const std::function<void(const QString &name)> &f);

        static void ForEachVarContainer(const std::function<void(const NoderVarProps::Container ctn)> &f);
        static void ForEachVarType(const std::function<void(const NoderVarProps::Type type)> &f);
        static void ForEachPinType(const std::function<void(const PinProperty::Type type)> &f);
        static void ForEachPinDirection(const std::function<void(const PinProperty::Direction direction)> &f);

        static const QByteArray &PlugValue(const NoderVarProps::Type type, bool linked);
        static const QByteArray &PlugArray(const NoderVarProps::Type type, bool linked);
        static const QByteArray &PlugExec(bool linked);

        static bool IsTypeCompatible(NoderVarProps::Type type1, NoderVarProps::Type type2);

        static void VarInfos(const NoderVarProps &varProps)
        {
            qDebug() << "Container" << Noder::VarContainerName(varProps.container);
            qDebug() << "Type     " << Noder::VarTypeName(varProps.type);
            qDebug() << "Subtype  " << varProps.subType;
        }

        static const QByteArray& (*PlugContainerFunc(NoderVarProps::Container ctn))(NoderVarProps::Type type, bool linked)
        {
            switch (ctn) {
                case NoderVarProps::Container::Reference: return PlugValue;
                case NoderVarProps::Container::Array: return PlugArray;
            };
            return nullptr;
        }

    private:
        Noder();

        static std::unordered_map<NoderVarProps::Type, s_varProperties> &VarTypeMap(void)
        {
            static std::unordered_map<NoderVarProps::Type, s_varProperties> map = {
                {NoderVarProps::Type::Bool, {"Boolean", "#E20909", true, false,
                    {
                        NoderVarProps::Type::Float,
                        NoderVarProps::Type::Int,
                        NoderVarProps::Type::String
                    }}
                },
                {NoderVarProps::Type::Int, {"Integer", "#19C5CC", true, false,
                    {
                        NoderVarProps::Type::Float,
                        NoderVarProps::Type::Int,
                        NoderVarProps::Type::String
                    }}
                },
                {NoderVarProps::Type::Wildcard, {"Wildcard", "#ffffff", false, false,
                    {
                        NoderVarProps::Type::Bool,
                        NoderVarProps::Type::Int,
                        NoderVarProps::Type::Float,
                        NoderVarProps::Type::String,
                        NoderVarProps::Type::Enum,
                        NoderVarProps::Type::Interface
                    }}
                },
                {NoderVarProps::Type::Float, {"Float", "#3AC242", true, false,
                    {
                        NoderVarProps::Type::Bool,
                        NoderVarProps::Type::Int,
                        NoderVarProps::Type::String
                    }}
                },
                {NoderVarProps::Type::String, {"String", "#C05DC2", true, false,
                    {
                        NoderVarProps::Type::Float,
                        NoderVarProps::Type::Int,
                        NoderVarProps::Type::Bool
                    }}
                },
                {NoderVarProps::Type::Enum, {"Enum", "#CE6135", false, true, {}}},
                {NoderVarProps::Type::Interface, {"Interface", "#C8B623", false, true, {}}},
            };
            return map;
        }

        static std::unordered_map<NoderVarProps::Container, QString> &VarContainerMap(void)
        {
            static std::unordered_map<NoderVarProps::Container, QString> map = {
                {NoderVarProps::Container::Reference, "Reference"},
                {NoderVarProps::Container::Array,    "Array"},
            };
            return map;
        }

        static std::unordered_map<NodeProperty::Type, QString> &NodeTypeMap(void)
        {
            static std::unordered_map<NodeProperty::Type, QString> map = {
                {NodeProperty::Type::Operation, "Operation"},
                {NodeProperty::Type::Exec, "Execution"},
                {NodeProperty::Type::Branch, "Branch"},
                {NodeProperty::Type::Instance, "Instance"},
                {NodeProperty::Type::Event, "Event"}
            };
            return map;
        }

        static std::unordered_map<PinProperty::Type, QString> &PinTypeMap(void)
        {
            static std::unordered_map<PinProperty::Type, QString> map = {
                {PinProperty::Type::Exec, "Exec"},
                {PinProperty::Type::Variable, "Value"}
            };
            return map;
        }

        static std::unordered_map<PinProperty::Direction, QString> &PinDirMap(void)
        {
            static std::unordered_map<PinProperty::Direction, QString> map = {
                {PinProperty::Direction::Input, "Input"},
                {PinProperty::Direction::Output, "Output"},
            };
            return map;
        }

        static std::map<QString, std::vector<QString>> &EnumMap(void)
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
        static QByteArray FillPlugMap(const NodeProperty::PlugType &plugType, const NoderVarProps::Type &type, bool linked);
        static QByteArray LoadCValue(const NoderVarProps::Type &type);
        static QByteArray LoadNcValue(const NoderVarProps::Type &type);
        static QByteArray LoadCArray(const NoderVarProps::Type &type);
        static QByteArray LoadNcArray(const NoderVarProps::Type &type);

        static inline std::unordered_map<NodeProperty::PlugType, struct NodeProperty::plugIcon> _mapPlugFiles;
        static inline std::unordered_map<NoderVarProps::Type, QByteArray> _mapPlugValueNcIcon;
        static inline std::unordered_map<NoderVarProps::Type, QByteArray> _mapPlugValueCIcon;
        static inline std::unordered_map<NoderVarProps::Type, QByteArray> _mapPlugArrayNcIcon;
        static inline std::unordered_map<NoderVarProps::Type, QByteArray> _mapPlugArrayCIcon;

        static inline QByteArray _plugExecNcIcon;
        static inline QByteArray _plugExecCIcon;
};