#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <QString>
#include <QColor>
#include <QVariant>

#include <PanduzaEngine.hpp>
#include <PzaMenu.hpp>

class Pin;
class GNode;

#define NBD_INST (NoderDataBase::GetInstance())

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

class NoderDataBase
{
    public:
        typedef std::function<GNode *(void)> t_CreateNode;

        NoderDataBase(NoderDataBase &other) = delete;
        void operator=(const NoderDataBase &) = delete;

        Pin *pinTypeToObj(const PinProperty::Type type);
        const QString &pinTypeToStr(const PinProperty::Type type);
        const QString &pinTypeToDir(const PinProperty::Direction direction);
        const QColor &plugColor(const PinProperty::Type type);
        const QColor &varColor(const NoderPanel::Type type);
        const QString &varTypeName(const NoderPanel::Type type);
        const std::vector<QString> &enumMap(const QString &name);
        const t_CreateNode &CreateNode(const QString &name);
        void initNodeMenuList(void);

        std::vector<PzaMenu *> &nodeMenuList(void) {return _nodeMenuList;}

        static NoderDataBase &GetInstance(void)
        {
            static NoderDataBase _database;

            return _database;
        }

        void forEachVarType(std::function<void(NoderPanel::Type type)> func);

        std::unordered_map<NoderPanel::Type, QString> _varTypeMap = {
            {NoderPanel::Type::Bool, "Boolean"},
            {NoderPanel::Type::Int,  "Integer"},
            {NoderPanel::Type::Float, "Float"},
            {NoderPanel::Type::String, "String"},
            {NoderPanel::Type::Enum,  "Enum"},
        };

    private:
        NoderDataBase();
        std::vector<PzaMenu *> _nodeMenuList;
};