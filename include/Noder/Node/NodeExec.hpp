#ifndef NODEEXEC_HPP
#define NODEEXEC_HPP

#include "Node.hpp"
#include <NodeEntryExec.hpp>

class NodeExec : public Node
{
    protected:
        NodeExec(NoderScene *scene, QString name, bool in, bool out)
            : Node(scene, name)
        {
            setTitleColor("#8F5709");

            _exec_in = (in) ? addNodeEntryInput<NodeEntryExec>() : nullptr;
            _exec_out = (out) ? addNodeEntryOutput<NodeEntryExec>() : nullptr;
        }
        NodeExec(NoderScene *scene, QString name)
            : NodeExec(scene, name, 1, 1)
        {
        
        }
        ~NodeExec() = default;

        void evaluateOperations(Node *node)
        {
            for (auto entry: node->entriesIn()) {
                if (entry->linked() && entry->type() != Exec) {
                    evaluateOperations(entry->linkedNode());
                    entry->linkedNode()->exec();
                }
            }
        }
        bool exec(void) override {return true;}
        
        Node *branch(void) override
        {
            Node *ret = nullptr;

            evaluateOperations(this);
            exec();
            for (auto entry: _entries_out) {
                ret = entry->execLinkedNode();
                if (ret)
                    break ;
            }
            return ret;
        }

    private:
        NodeEntryExec *_exec_in;
        NodeEntryExec *_exec_out;
};

#endif