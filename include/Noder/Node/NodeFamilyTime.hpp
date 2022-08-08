#ifndef NODEFAMILYTIME_HPP
#define NODEFAMILYTIME_HPP

#include <QThread>

#include <NodeExec.hpp>
#include <NodeEntryDouble.hpp>

class N_Time_Delay : public NodeExec
{
    public:
        N_Time_Delay(NoderScene *scene)
            : NodeExec(scene, "Delay")
        {
            value = addNodeEntryInput<NodeEntryDouble>("Value");
        }
        ~N_Time_Delay() = default;

        bool exec(void) override
        {
            double ms = value->value() * 1000;
            QThread::msleep(ms);
        
            return true;
        }   

    private:
        NodeEntryDouble *value;
};
#endif