#include <Link.hpp>
#include <GNode.hpp>

Link::Link(Pin *from)
    : QGraphicsObject(),
    _inpos(0, 0),
    _outpos(0, 0),
    _inPin(nullptr),
    _outPin(nullptr),
    _candidate(nullptr)
{
    GNode *node;

    node = from->node();
    _scene = node->scene();
    _scenePos = node->mapToScene(from->plugCenter());

    setFlag(QGraphicsItem::ItemIsFocusable, true);

    _scene->addItem(this);

    setPin(from);
    setPos(_scenePos);

    setZValue(-1);

    _offpen = QPen(Qt::gray, 2.0, Qt::DashLine, Qt::RoundCap);
}

Link::Link(Pin *from, Pin *to)
    : Link(from)
{
    setPin(to);
}

void Link::kill(void)
{
    if (_inPin && _inPin->dead() == false)
        _inPin->removeLink(this);
    if (_outPin && _outPin->dead() == false)
        _outPin->removeLink(this);
    deleteLater();
}

void Link::setPin(Pin *pin)
{
    switch (pin->direction()) {
        case PinProperty::Direction::Input:
            _outPin = pin;
            break ;
        case PinProperty::Direction::Output:
            _inPin = pin;
            break ;
    }
}

bool Link::isCompatible(Pin *to)
{
    if (_inPin && 
        to->direction() == PinProperty::Direction::Input
        && to->node() != _inPin->node()
        && to->isCompatible(_inPin)
    )
        return true;

    if (_outPin && 
        to->direction() == PinProperty::Direction::Output
        && to->node() != _outPin->node()
        && to->isCompatible(_outPin)
    )
        return true;

    return false;
}

void Link::moveDrag(const QPointF &pos)
{
    if (_inPin)
        _outpos = pos;
    else
        _inpos = pos;
}

void Link::move(void)
{
    prepareGeometryChange();

    _inpos = mapFromScene(_inPin->node()->mapToScene(_inPin->plugCenter()));
    _outpos = mapFromScene(_outPin->node()->mapToScene(_outPin->plugCenter()));

    update();
}

void Link::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);

    if (event->key() == Qt::Key_Escape) {
        ungrabMouse();
        deleteLater();
    }
}

void Link::setLinkComplete(void)
{
    setPin(_candidate);
    _inPin->connectLink(this);
    _outPin->connectLink(this);
}

void Link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    findCandidate(event->scenePos());
    QGraphicsItem::mousePressEvent(event);
}

void Link::searchCandidate(GNode *node, const QPoint &pos)
{
    node->forEachPin([&](Pin *pin) {
        if (pin->isInPlugArea(pos)
            && isCompatible(pin)
            && ((pin->isInput() && !pin->linked()) || pin->isOutput())) {
                moveDrag(mapFromScene(node->mapToScene(pin->plugCenter())));
                _candidate = pin;
                return ;
            }
    });
}

void Link::findCandidate(const QPointF &pos)
{
    QPoint posInNode;
    GNode *node;

    _candidate = nullptr;
    node = _scene->findNodeAt(pos);
    if (node) {
        posInNode = node->mapFromScene(pos).toPoint();
        if (node->isInPlugzone(posInNode)) {
            searchCandidate(node, posInNode);
        }
    }
}

void Link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    moveDrag(event->pos());
    findCandidate(event->scenePos());
    prepareGeometryChange();
    update();
}

void Link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    findCandidate(event->scenePos());
    ungrabMouse();
    ungrabKeyboard();

    if (_candidate) {
        setLinkComplete();
        prepareGeometryChange();
        update();
    }
    else {
        deleteLater();
    }
}

void Link::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    (void)option;
    (void)w;

    if (_inPin && _inPin->dead() == false && _outPin && _outPin->dead() == false) {
        QColor col_in;
        QColor col_out;
        QPen pen;

        if (_inPin->type() == PinProperty::Type::Exec) {
            pen.setColor("#cccccc");
            pen.setWidth(3.0);
            pen.setStyle(Qt::DashLine);
        }
        else {
            QLinearGradient gradient(_inpos, _outpos);
            col_in = static_cast<PinVariable *>(_inPin)->plugColor();
            col_out = static_cast<PinVariable *>(_outPin)->plugColor();
            gradient.setColorAt(0, col_in);
            gradient.setColorAt(1, col_out);
            pen.setBrush(QBrush(gradient));
            pen.setWidth(2.5);
        }
        painter->setPen(pen);
        }
    else
        painter->setPen(_offpen);
    painter->drawLine(_outpos, _inpos);
}

Link::~Link()
{

}