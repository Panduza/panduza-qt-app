#ifndef NODEBRANCH_HPP
#define NODEBRANCH_HPP

#include "NodeExec.hpp"

class NodeBranch : public NodeExec
{
    public:
        NodeBranch(NoderScene *scene, QString name)
            : NodeExec(scene, name, 1, 0)
        {
            setTitleColor("#757575");
        }
        ~NodeBranch() = default;
};

#endif