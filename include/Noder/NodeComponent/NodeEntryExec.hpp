#ifndef NODEENTRYEXEC_HPP
#define NODEENTRYEXEC_HPP

#include <QPainter>
#include <NodeEntry.hpp>

class NodeEntryExec : public NodeEntry
{
    public:
        NodeEntryExec(QString name, enum NodeEntryDirection direction, Node *parent)
            : NodeEntry(name, direction, parent)
        {
            _type = Exec;
            setPlugColor("#EEEEEE");

            _exec_on.addPixmap(QPixmap(":images/noder/execplug_on.png"));
            _exec_off.addPixmap(QPixmap(":images/noder/execplug_off.png"));
        }
        ~NodeEntryExec() = default;

        Node *execLinkedNode(void) override {return linkedNode();}
        void drawPlug(QPainter *painter) override
        {
            QIcon *icon;

            icon = (linked()) ? &_exec_on : &_exec_off;
            icon->paint(painter, plugzone().toRect());
        };
    
    private:
        QIcon _exec_on;
        QIcon _exec_off;
};

#endif