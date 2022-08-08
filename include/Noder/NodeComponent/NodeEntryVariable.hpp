#ifndef NODEENTRYVARIABLE_HPP
#define NODEENTRYVARIABLE_HPP

#include <QPainter>
#include <NodeEntry.hpp>

class NodeEntryVariable : public NodeEntry
{
    protected:
        NodeEntryVariable(QString name, enum NodeEntryDirection direction, Node *parent)
            : NodeEntry(name, direction, parent)
        {

        }
        ~NodeEntryVariable() = default;

        void drawPlug(QPainter *painter) override
        {
            if (linked()) {
                painter->setPen(QPen(plugcolor(), 1.0));
                painter->setBrush(QBrush(plugcolor()));
            }
            else {
                painter->setPen(QPen(plugcolor(), 2.0));
                painter->setBrush(QColor("#0D0D0D"));
            }
            painter->drawEllipse(plugCenter(), 4, 4);
        };

};

#endif