#ifndef NODECOMPONENT_HPP
#define NODECOMPONENT_HPP

#include <QPoint>
#include <QSize>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>

class Node;

class NodeComponent : public QWidget
{
    public:
        NodeComponent(QString name, Node *parent);
        ~NodeComponent() = default;

        void setSize(QSize size) { _size = size; }
        QSize size(void) const { return _size; }
 
        void setPos(QPoint pos) { _pos = pos; }
        QPoint pos(void) const { return _pos; }

        QGraphicsProxyWidget *proxy(void) const { return _proxy; }

        QString label(void) const {return _name->text();}

    protected:
        virtual void addEntryName(QString name) {
            _name = new QLabel(name);
            _name->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            _layout->addWidget(_name, 0, 0, Qt::AlignLeft);
        };

        QGraphicsProxyWidget *_proxy;
        QGridLayout *_layout;
        QLabel *_name;
        QSize _size;
        Node *_node;
        QPoint _pos;
        int _type;
};

#endif