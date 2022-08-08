#ifndef NODEEVENT_HPP
#define NODEEVENT_HPP

#include "NodeExec.hpp"

class NodeEvent : public NodeExec
{
    protected:
        NodeEvent(NoderScene *scene, QString name)
            : NodeExec(scene, name, 0, 1)
        {
            setTitleColor("#92271C");
        }
        ~NodeEvent() = default;
};

#endif