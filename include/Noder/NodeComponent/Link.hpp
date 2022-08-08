#ifndef LINK_HPP
#define LINK_HPP

#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <NoderScene.hpp>
#include <NodeEntry.hpp>

class Link : public QGraphicsObject
{
    public:
        Link(NoderScene *scene, QPointF pos, NodeEntry *from);
        ~Link();

        void setLinkComplete(void);
        void setEntry(NodeEntry *entry);
        bool isCompatible(NodeEntry *to);
        void moveDrag(QPointF pos);
        void move(void);
        void disconnect(NodeEntry *dead);
        void keyPressEvent(QKeyEvent *event) override;
        NodeEntry *inEntry(void) {return _in_entry;}
        NodeEntry *outEntry(void) {return _out_entry;}
        void entryDied(NodeEntry *entry);

    private:
        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

        QRectF boundingRect() const override { return QRectF(_outpos, _inpos).normalized(); }
        void paint(QPainter *, QStyleOptionGraphicsItem const *, QWidget *) override;

        QPen _onpen;
        QPen _offpen;

        NoderScene *_scene;
        QPointF _scenepos;
        NodeEntry *_in_entry;
        NodeEntry *_out_entry;
        NodeEntry *_candidate;
        QPointF _inpos;
        QPointF _outpos;

        void findCandidate(QPointF pos);
        void searchCandidate(Node *node, QPoint pos);
};

#endif