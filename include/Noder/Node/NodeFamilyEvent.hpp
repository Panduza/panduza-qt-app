#ifndef NODEFAMILYEVENT_HPP
#define NODEFAMILYEVENT_HPP

#include <NodeEvent.hpp>

class N_Event_Start : public NodeEvent
{
    public:
        N_Event_Start(NoderScene *scene)
            : NodeEvent(scene, "Event Start")
        {

        }
        ~N_Event_Start() = default;
};

#endif