#ifndef NODEFAMILYBRANCH_HPP
#define NODEFAMILYBRANCH_HPP

#include <NodeBranch.hpp>
#include <NodeEntryInteger.hpp>
#include <NodeEntryBool.hpp>

class N_Branch_If : public NodeBranch
{
    public:
        N_Branch_If(NoderScene *scene)
            : NodeBranch(scene, "If")
        {
            _out_true = addNodeEntryOutput<NodeEntryExec>("True");
            _out_false = addNodeEntryOutput<NodeEntryExec>("False");
            _cond = addNodeEntryInput<NodeEntryBool>("Condition");
        }
        ~N_Branch_If() = default;

        Node *branch(void) override
        {
            return (_cond->state()) ? _out_true->linkedNode() : _out_false->linkedNode();
        }

    private:
        NodeEntryBool *_cond;
        NodeEntryExec *_out_true;
        NodeEntryExec *_out_false;
};

class N_Branch_For : public NodeBranch
{
    public:
        N_Branch_For(NoderScene *scene)
            : NodeBranch(scene, "For")
        {
            _start = addNodeEntryInput<NodeEntryInteger>("Start");
            _end = addNodeEntryInput<NodeEntryInteger>("End");
            _scope = addNodeEntryOutput<NodeEntryExec>("Scope");
            _done = addNodeEntryOutput<NodeEntryExec>("Done");
            _index = addNodeEntryOutput<NodeEntryInteger>("Index");
        }
        ~N_Branch_For() = default;

        Node *branch(void) override
        {
            // Warn if start > end
            int start;
            int end;

            start = _start->value();
            end = _end->value();
            for (int i = start; i < end; i++) {
                _index->sendValue(i);
                scene()->executeSection(_scope->linkedNode());
            }
            return _done->linkedNode();
        }

    private:
        NodeEntryInteger *_start;
        NodeEntryInteger *_end;
        NodeEntryExec *_scope;
        NodeEntryExec *_done;
        NodeEntryInteger *_index;
};

#endif