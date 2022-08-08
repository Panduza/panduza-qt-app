#ifndef NODEFAMILYSTRING_HPP
#define NODEFAMILYSTRING_HPP

#include <NodeExec.hpp>
#include <NodeOperation.hpp>
#include <NodeEntryString.hpp>
#include <NodePropertyPin.hpp>

class N_String_Common
{
    public:
        static void addPinString(Node *node, std::vector<NodeEntryString *> &_list)
        {
            NodeEntryString *entry;

            entry = node->addNodeEntryInput<NodeEntryString>("String " + QString::number(_list.size() + 1));
           _list.push_back(entry);
        }
        
        static void removePinString(Node *node, std::vector<NodeEntryString *> &_list, const int min)
        {
            if (_list.size() <= min)
                return ;
            node->deleteEntry(_list.back());
            _list.pop_back();
        }

    private:
        N_String_Common() = delete;
        ~N_String_Common() = delete;
};

class N_String_Print : public NodeExec
{
    public:
        N_String_Print(NoderScene *scene)
            : NodeExec(scene, "Print")
        {
            txt = addNodeEntryInput<NodeEntryString>("Text");
        }
        ~N_String_Print() = default;

        bool exec(void) override
        {
            qDebug() << txt->text();
            return true;
        }

    private:
        NodeEntryString *txt;
};

class N_String_Append : public NodeOperation
{
    public:
        N_String_Append(NoderScene *scene)
            : NodeOperation(scene, "Append")
        {
            NodePropertyPin *addpin = addNodeProperty<NodePropertyPin>();

            connect(addpin, &NodePropertyPin::more, this, [&]{N_String_Common::addPinString(this, _strings);});
            connect(addpin, &NodePropertyPin::less, this, [&]{N_String_Common::removePinString(this, _strings, 2);});

            _separator = addNodeEntryInput<NodeEntryString>("Separator");
            _result = addNodeEntryOutput<NodeEntryString>("Result");
            N_String_Common::addPinString(this, _strings);
            N_String_Common::addPinString(this, _strings);
        }
        ~N_String_Append() = default;

        bool exec(void) override
        {
            QString res;
            size_t len;

            len = _strings.size();
            for (int i = 0; i < len; i++) {
                res = res + _strings[i]->text();
                if (i + 1 != len)
                    res = res + _separator->text();
            }
            _result->sendValue(res);
            return true;
        }

    private:
        NodeEntryString *_separator;
        NodeEntryString *_result;
        std::vector<NodeEntryString *> _strings;
};

#endif