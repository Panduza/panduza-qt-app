#ifndef NODEPROPERTYPIN_HPP
#define NODEPROPERTYPIN_HPP

#include <QPushButton>

#include <NodeProperty.hpp>

class NodePropertyPin: public NodeProperty
{
    Q_OBJECT
    
    public:
        NodePropertyPin(QString name, Node *parent);
        NodePropertyPin(Node *parent);
        ~NodePropertyPin();

    private:
        QPushButton *_more_btn;
        QPushButton *_less_btn;

    signals:
        void more(void);
        void less(void);
};

#endif