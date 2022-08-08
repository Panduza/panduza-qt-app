#ifndef NODEFAMILYINTERFACE_HPP
#define NODEFAMILYINTERFACE_HPP

#include <NodeExec.hpp>
#include <NodeEntryString.hpp>
#include <NodeEntryInterface.hpp>

class N_Interface_Find : public NodeExec
{
    public:
        N_Interface_Find(NoderScene *scene)
            : NodeExec(scene, "Find Interface")
        {
            _machine = addNodeEntryInput<NodeEntryString>("Machine");
            _group = addNodeEntryInput<NodeEntryString>("Group");
            _name = addNodeEntryInput<NodeEntryString>("Name");
            _interface = addNodeEntryOutput<NodeEntryInterface>("Interface");
        }
        ~N_Interface_Find() = default;

        bool exec(void) override
        {
            Interface *interface = scene()->engine()->findInterface(_machine->text(), _group->text(), _name->text());
            
            if (interface)
                qDebug() << interface->name();
            else
                qDebug() << "Not found";

            _interface->sendValue(interface);
            return true;
        }
    private:
        NodeEntryString *_machine;
        NodeEntryString *_group;
        NodeEntryString *_name;
        NodeEntryInterface *_interface;
};

#endif