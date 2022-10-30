#pragma once

#include <unordered_map>
#include <string>
#include <vector>

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
        Wildcard = 0,
        Int,
        Float,
        Bool,
        String,
        Enum,
        Array,
        Interface,
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
};

struct NoderPanel
{
    enum class Type
    {
        Bool,
        Int,
        Float,
        String,
        Enum
    };
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

        NoderFrame *Frame = nullptr;
        
        Noder(Noder &other) = delete;
        void operator=(const Noder &) = delete;

        typedef std::function<GNode *(void)> t_CreateNode;

        Pin *pinTypeToObj(const PinProperty::Type type);
        const QString &pinTypeToStr(const PinProperty::Type type);
        const QString &pinTypeToDir(const PinProperty::Direction direction);
        const QColor &plugColor(const PinProperty::Type type);
        const QColor &varColor(const NoderPanel::Type type);
        const QString &varTypeName(const NoderPanel::Type type);
        const QString &nodeTypeName(const NodeProperty::Type type);
        const std::vector<QString> &enumValues(const QString &name);

        void forEachEnum(const std::function<void(const QString &name, const std::vector<QString> &list)> &f);
        void forEachEnumName(const std::function<void(const QString &name)> &f);
        void forEachEnumValues(const QString &name, const std::function<void(const QString &name)> &f);

        void forEachVarType(const std::function<void(NoderPanel::Type type)> &f);
        NoderPanel::Type varTypeFromName(const QString &name);
        PinProperty::Type panelTypeToPinType(const NoderPanel::Type type);

        std::unordered_map<NoderPanel::Type, QString> _varTypeMap = {
            {NoderPanel::Type::Bool, "Boolean"},
            {NoderPanel::Type::Int,  "Integer"},
            {NoderPanel::Type::Float, "Float"},
            {NoderPanel::Type::String, "String"},
            {NoderPanel::Type::Enum,  "Enum"},
        };

        std::map<QString, std::vector<QString>> _enumMap = {
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

    private:
        Noder();
        std::vector<PzaMenu *> _nodeMenuList;
};