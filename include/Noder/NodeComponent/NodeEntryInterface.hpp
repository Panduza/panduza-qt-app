#ifndef NODEENTRYINTERFACE_HPP
#define NODEENTRYINTERFACE_HPP

#include <NodeEntryVariable.hpp>
#include <Interface.hpp>

class NodeEntryInterface : public NodeEntryVariable
{
    public:
        NodeEntryInterface(QString name, enum NodeEntryDirection direction, Node *parent);
        ~NodeEntryInterface();

        Interface *object(void) {return _interface;}

        void setValue(Interface *val) override {_interface = val;};

    private:
        Interface *_interface = nullptr;
};

#endif