#ifndef NODEENTRYINTEGER_HPP
#define NODEENTRYINTEGER_HPP

#include <QSpinBox>

#include <NodeEntryVariable.hpp>

class NodeEntryInteger : public NodeEntryVariable
{
    public:
        NodeEntryInteger(QString name, enum NodeEntryDirection direction, Node *parent);
        ~NodeEntryInteger();

        double const value(void) {return _valuebox->value();}
        void setValue(double val) {_valuebox->setValue(val);}

    private:
        QSpinBox *_valuebox;
};

#endif