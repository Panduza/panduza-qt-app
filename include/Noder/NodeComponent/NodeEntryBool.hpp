#ifndef NODEENTRYBOOL_HPP
#define NODEENTRYBOOL_HPP

#include <NodeEntryVariable.hpp>
#include <QCheckBox>

class NodeEntryBool : public NodeEntryVariable
{
    public:
        NodeEntryBool(QString name, enum NodeEntryDirection direction, Node *parent);
        ~NodeEntryBool();

        bool state() {return (linked()) ? _value : _checkbox->isChecked();};

        void setValue(double val) override {_value = (bool)val;}
        void setValue(bool val) override {_value = val;}
        void setValue(int val) override {_value = (bool)val;};

    private:
        QCheckBox *_checkbox;
        bool _value;
};

#endif