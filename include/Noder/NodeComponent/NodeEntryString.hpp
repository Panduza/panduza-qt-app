#ifndef NODEENTRYSTRING_HPP
#define NODEENTRYSTRING_HPP

#include <QLineEdit>

#include <NodeEntryVariable.hpp>

class NodeEntryString : public NodeEntryVariable
{
    public:
        NodeEntryString(QString name, enum NodeEntryDirection direction, Node *parent);
        ~NodeEntryString();

        void setText(QString val) {_value = val;};
        QString text(void) const {return (linked()) ? _value : _txtbox->text();};

        void setValue(double val) override {setText(QString::number(val));};
        void setValue(int val) override {setValue((double)val);};
        void setValue(bool val) override {setValue((bool)val);};
        void setValue(QString val) override {setText(val);};

    private:
        QLineEdit *_txtbox;
        QString _value;
};

#endif