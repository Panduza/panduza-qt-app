#ifndef NODEENTRY_HPP
#define NODEENTRY_HPP

#include <NodeComponent.hpp>

class Node;
class Link;
class Interface;

enum NodeEntryDirection {
    Input,
    Output
};

enum NodeEntryType {
    NoType = 0,
    Exec,
    Bool,
    Integer,
    Double,
    String,
    InterfaceType,
    NodeEntryTypeMax
};

class NodeEntry : public NodeComponent
{
    public:
        ~NodeEntry();
        void createLink(void);
        void connectLink(Link *link);
        void moveLink(void);
        void disconnectLink(void);
        void unlink(void);

        bool linked(void) const { return _link; }

        void hideWidgets(void);
        void showWidgets(void);

        virtual Node *execLinkedNode() {return nullptr;};

        void setDirection(enum NodeEntryDirection direction) { _direction = direction; }
        enum NodeEntryDirection direction(void) const { return _direction; }

        void setType(enum NodeEntryType type) { _type = type; }
        enum NodeEntryType type(void) const { return _type; }

        void setPlugzone(QRectF plug) { _plugzone = plug; }
        QRectF plugzone(void) const { return _plugzone; }

        bool isInPlugArea(QPointF pos) const { return _plugzone.contains(pos); }

        QPointF plugCenter(void) const { return _plugzone.center(); }

        Node *linkedNode(void);
        NodeEntry *linkedEntry(void);

        void setNode(Node * node) { _node = node; }
        Node *node(void) const { return _node; }

        QColor &plugcolor() {return _plugcolor;}
        void setPlugColor(QString color) {_plugcolor = QColor(color);}

        template<typename T>
        void sendValue(T val) {
            if (linked())
                linkedEntry()->setValue(val);
        }

        virtual void drawPlug(QPainter *painter) = 0;

        void setLink(Link *link) { _link = link; }

    protected:
        NodeEntry(QString name, enum NodeEntryDirection direction, Node *parent);
        NodeEntry(enum NodeEntryDirection direction, Node *parent);

        enum NodeEntryType _type;
        enum NodeEntryDirection _direction;

        template<class T>
        T *addWidget(int x, int y)
        {
            T *w = nullptr;

            if (_direction != Output) {
                w = new T(this);
                w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
                _layout->addWidget(w, x, y);
            }
            return w;
        };

        void addEntryName(QString name) override {
            _name = new QLabel(name);
            switch (_direction) {
                case NodeEntryDirection::Input:
                    _layout->addWidget(_name, 0, 0, Qt::AlignLeft);
                    break;
                case NodeEntryDirection::Output:
                    _layout->addWidget(_name, 0, 0, Qt::AlignRight);
                    break;
            }
            _name->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        };

        virtual void setValue(bool val) {qDebug() << "Wooh bad idea";};
        virtual void setValue(double val) {qDebug() << "Wooh bad idea";};
        virtual void setValue(int val) {qDebug() << "Wooh bad idea";};
        virtual void setValue(QString val) {qDebug() << "Wooh bad idea";};
        virtual void setValue(Interface *val) {qDebug() << "Wooh bad idea";};

    private:
        Link *_link;
        QSize _size;
        QRectF _plugzone;
        QColor _plugcolor;
        QPoint _pos;
};

#endif