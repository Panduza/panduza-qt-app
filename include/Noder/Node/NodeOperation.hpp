#ifndef NODEOPERATION_HPP
#define NODEOPERATION_HPP

#include "Node.hpp"

class NodeOperation : public Node
{
    protected:
        NodeOperation(NoderScene *scene, QString name)
            : Node(scene, name)
        {
            setTitleColor("#307146");
        }
        ~NodeOperation() = default;

        Node *branch(void) override {return nullptr;}
};

#endif