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

class NoderDataBase
{
    protected:
        NoderDataBase();

    public:
        typedef std::function<GNode *(void)> t_createNode;

        NoderDataBase(NoderDataBase &other) = delete;
        void operator=(const NoderDataBase &) = delete;

        Pin *pinTypeToObj(const PinProperty::Type type);
        const QString &pinTypeToStr(const PinProperty::Type type);
        const QString &pinTypeToDir(const PinProperty::Direction direction);
        const QColor &plugColor(const PinProperty::Type type);
        const std::vector<QString> &enumMap(const QString &name);
        const t_createNode &createNode(const QString &name);
        void initNodeMenuList(void);

        std::vector<PzaMenu *> &nodeMenuList(void) {return _nodeMenuList;}

        static NoderDataBase *GetInstance(void)
        {
            static NoderDataBase *_database;

            if(_database == nullptr)
                _database = new NoderDataBase();
            return _database;
        }

        static NoderDataBase &GetInstance(void)
        {
            static NoderDataBase _database;
            

            // if(_database == nullptr)
                // _database = new NoderDataBase();
            return _database;
        }

    private:
        std::vector<PzaMenu *> _nodeMenuList;
};