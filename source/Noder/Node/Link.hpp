#pragma once

#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <NoderScene.hpp>
#include <Pin.hpp>

class Link : public QGraphicsObject
{
    public:
        Link(Pin *from);
        Link(Pin *from, Pin *to);
        ~Link();

        void setLinkComplete(void);
        void setPin(Pin *pin);
        bool isCompatible(Pin *to);
        void moveDrag(const QPointF &pos);
        void move(void);
        void keyPressEvent(QKeyEvent *event) override;
        Pin *inPin(void) {return _inPin;}
        Pin *outPin(void) {return _outPin;}
        Pin *oppositePin(const Pin *pin) {return (pin == _inPin) ? _outPin : _inPin;}
        void kill(void);

    private:
        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

        QRectF boundingRect() const override { return QRectF(_outpos, _inpos).normalized(); }
        void paint(QPainter *, QStyleOptionGraphicsItem const *, QWidget *) override;

        QPen _onpen;
        QPen _offpen;

        NoderScene *_scene;
        QPointF _scenePos;
        QPointF _inpos;
        QPointF _outpos;
        Pin *_inPin;
        Pin *_outPin;
        Pin *_candidate;

        void findCandidate(const QPointF &pos);
        void searchCandidate(GNode *node, const QPoint &pos);
};