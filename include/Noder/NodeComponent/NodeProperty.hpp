#ifndef NODEPROPERTY_HPP
#define NODEPROPERTY_HPP

#include <NodeComponent.hpp>

class Node;

class NodeProperty : public NodeComponent
{
    public:
        NodeProperty(QString name, Node *parent) : NodeComponent(name, parent) {};
        ~NodeProperty() = default;

    protected:
    
        template<class T>
        T *addWidget(int x, int y)
        {
            T *w = new T(this);
            w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            _layout->addWidget(w, x, y);
            return w;
        };
};

#endif