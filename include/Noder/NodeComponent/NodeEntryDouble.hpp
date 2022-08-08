#ifndef NODEENTRYDOUBLE_HPP
#define NODEENTRYDOUBLE_HPP

#include <QDoubleSpinBox>

#include <NodeEntryVariable.hpp>

class NodeEntryDouble : public NodeEntryVariable
{
    public:
        NodeEntryDouble(QString name, enum NodeEntryDirection direction, Node *parent);
        ~NodeEntryDouble();

        double value(void) {return (linked()) ? _value : _valuebox->value();}

        void setValue(double val) override {_value = val;};
        void setValue(bool val) override {_value = (double)val;};
        void setValue(int val) override {_value = (double)val;};
        void setValue(QString val) override
        {
            bool ok;
            double res = val.toDouble(&ok);
            if (ok) {
                _value = res;
            }
            else
                qWarning() << "Input string" << val << "cannot be converted to double.. The result will be undefined";
                ;
                // generate warning to tell the conversion is not possible
        };

    private:
        QDoubleSpinBox *_valuebox;
        double _value;
};

#endif