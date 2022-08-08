#include <Link.hpp>
#include <Node.hpp>

Link::Link(NoderScene *scene, QPointF pos, NodeEntry *from)
    : QGraphicsObject(),
    _scene(scene),
    _scenepos(pos),
    _inpos(0, 0),
    _outpos(0, 0),
    _in_entry(nullptr),
    _out_entry(nullptr),
    _candidate(nullptr)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    _scene->addItem(this);

    setEntry(from);
    setPos(_scenepos);

    setZValue(-1);

    _offpen = QPen(Qt::gray, 2.0, Qt::DashLine, Qt::RoundCap);
}

void Link::setEntry(NodeEntry *entry)
{
    switch (entry->direction()) {
        case NodeEntryDirection::Input:
            _out_entry = entry;
            break ;
        case NodeEntryDirection::Output:
            _in_entry = entry;
            break ;
    }
}

bool Link::isCompatible(NodeEntry *to)
{
    // Is compatible if we connect an output to an input,
    // and if it's not part of the same node.

    // In the future it should (must) also check if the type is comaptible (int to into, string to string..)

    if (_in_entry && 
        to->direction() == NodeEntryDirection::Input
        && to->node() != _in_entry->node()
    //    && to->getType() == _in_entry->getType()
    )
        return true;

    if (_out_entry && 
        to->direction() == NodeEntryDirection::Output
        && to->node() != _out_entry->node()
 //       && (to->getType() == _out_entry->getType())
    )
        return true;

    return false;
}

void Link::moveDrag(QPointF pos)
{
    if (_in_entry)
        _outpos = pos;
    else
        _inpos = pos;
}

void Link::move(void)
{
    prepareGeometryChange();

    _inpos = mapFromScene(_in_entry->node()->mapToScene(_in_entry->plugCenter()));
    _outpos = mapFromScene(_out_entry->node()->mapToScene(_out_entry->plugCenter()));

    update();
}

void Link::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);

    if (event->key() == Qt::Key_Escape) {
        ungrabMouse();
        delete this;
    }
}

void Link::disconnect(NodeEntry *dead)
{
    if (dead == _in_entry) {
        _out_entry->unlink();
        _in_entry = nullptr;
    }
    else if (dead == _out_entry) {
        _in_entry->unlink();
        _out_entry = nullptr;
    }
    update();
}

void Link::entryDied(NodeEntry *entry)
{
    Link *link;

    if (_in_entry == entry)
        _out_entry->unlink();
    else if (_out_entry == entry)
        _in_entry->unlink();

    _in_entry = nullptr;
    _out_entry = nullptr;

    delete this;
}

void Link::setLinkComplete(void)
{
    setEntry(_candidate);
    _in_entry->connectLink(this);
    _out_entry->connectLink(this);
}

void Link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    findCandidate(event->scenePos());
    QGraphicsItem::mousePressEvent(event);
}

void Link::searchCandidate(Node *node, QPoint pos)
{
    for (auto entry: node->entriesIn()) {
        if (entry->isInPlugArea(pos) && isCompatible(entry) && !entry->linked()) {
            moveDrag(mapFromScene(node->mapToScene(entry->plugCenter())));
            _candidate = entry;
            return ;
        }
    }
    for (auto entry: node->entriesOut()) {
        if (entry->isInPlugArea(pos) && isCompatible(entry) && !entry->linked()) {
            moveDrag(mapFromScene(node->mapToScene(entry->plugCenter())));
            _candidate = entry;
            return ;
        }
    }
}

void Link::findCandidate(QPointF pos)
{
    QPoint pos_in_node;
    Node *node;

    _candidate = nullptr;
    node = _scene->findNodeAt(pos);
    if (node) {
        pos_in_node = node->mapFromScene(pos).toPoint();
        if (node->isInPlugzone(pos_in_node)) {
            searchCandidate(node, pos_in_node);
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

    if (_candidate)
        setLinkComplete();
    else {
        if (_in_entry)
            _in_entry->unlink();
        if (_out_entry)
            _out_entry->unlink();
        _scene->removeItem(this);
        deleteLater();
    }

    prepareGeometryChange();
    update();
}

void Link::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    if (_in_entry && _out_entry) {
        QColor col_in;
        QColor col_out;
        QPen pen;
        QLinearGradient gradient(_inpos, _outpos);

        col_in = _in_entry->plugcolor();
        col_out = _out_entry->plugcolor();
        gradient.setColorAt(0, col_in);
        gradient.setColorAt(1, col_out);
        pen.setBrush(QBrush(gradient));
        if (_in_entry->type() == Exec) {
            pen.setWidth(3.0);
            pen.setStyle(Qt::DashLine);
        }
        else
            pen.setWidth(2.0);
        painter->setPen(pen);
    }
    else
        painter->setPen(_offpen);
    painter->drawLine(_outpos, _inpos);
}

Link::~Link()
{

}