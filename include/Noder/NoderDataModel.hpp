#ifndef NODERDATAMODEL_HPP
#define NODERDATAMODEL_HPP

#include <PanduzaEngine.hpp>
#include <PzaMenu.hpp>
#include <NodeFamilies.hpp>

class NoderDataModel
{
    public:
        NoderDataModel(PanduzaEngine *engine);
        ~NoderDataModel() = default;

        std::vector<PzaMenu *> &nodeMenuList(void) {return _nodeMenuList;}

        typedef std::function<Node *(NoderScene *scene)> t_createChildNode;
        typedef std::unordered_map<QString, std::map<QString, t_createChildNode>> t_nodemap;

    private:
        std::vector<PzaMenu *> _nodeMenuList;
        PanduzaEngine *_engine;

        void AddNodeToMenu(PzaMenu *toMenu, const QString &name, t_createChildNode);
        void initNodeMenuList(void);
};

#endif