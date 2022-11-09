#include <GNode.hpp>

#include <Noder.hpp>
#include <NoderScene.hpp>
#include <Link.hpp>
#include <NoderView.hpp>

GNode::GNode(const QString &name)
    : QGraphicsObject(),
    _plugzone(20),
    _nodebox(0, 0, 0, 0),
    _pinBox(0, 0, 0, 0),
    _entryMiny(25),
    _pinBoxOffsetY(10),
    _name(name),
    _userName(name),
    _boxRadius(6),
    _boxColor("#353535")
{
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setCacheMode(QGraphicsItem::NoCache);
    setAcceptHoverEvents(true);
    setOpacity(0.9f);
    setZValue(0);

    setOnTop();

    _propTable = new PzaPropertyTable();
}

void GNode::setColor(const QColor &color)
{
    _boxColor = color;
    refreshNode();
}

void GNode::setType(NodeProperty::Type type)
{
    _type = type;
    _propType->setText(Noder::Get().nodeTypeName(_type));
}

void GNode::refreshUserName(const QString &name)
{
    _userName = name;
    refreshNode();
}

void GNode::forEachInputPin(const std::function<void(Pin *pin)> &func)
{
    for (auto const &pin: _inputPins) {
        func(pin);
    }
}

void GNode::forEachOutputPin(const std::function<void(Pin *pin)> &func)
{
    for (auto const &pin: _outputPins) {
        func(pin);
    }
}

void GNode::forEachPin(const std::function<void(Pin *pin)> &func)
{
   forEachInputPin(func);
   forEachOutputPin(func);
}

void GNode::setScene(NoderScene *scene)
{
    scene->addItem(this);
    _scene = scene;
}

PinValue *GNode::addPinFromType(NoderVar::Type type, const QString &name, PinProperty::Direction direction, int index)
{
    PinValue *pin = nullptr;

    switch (type) {
        case NoderVar::Type::Bool:       pin = addPin<PinDecl::Bool>(name, direction, index); break ;
        case NoderVar::Type::Float:      pin = addPin<PinDecl::Float>(name, direction, index); break ;
        case NoderVar::Type::Int:        pin = addPin<PinDecl::Int>(name, direction, index); break ;
        case NoderVar::Type::String:     pin = addPin<PinDecl::String>(name, direction, index); break ;
        case NoderVar::Type::Wildcard:   pin = addPin<PinDecl::Wildcard>(name, direction, index); break ;
        case NoderVar::Type::Enum:       pin = addPin<PinDecl::Enum>(name, direction, index); break ;
        case NoderVar::Type::Array:      pin = addPin<PinDecl::Array>(name, direction, index); break ;
        case NoderVar::Type::Interface:  pin = addPin<PinDecl::Interface>(name, direction, index); break ;
    }
    return pin;
}

void GNode::addProxyWidget(Pin *pin)
{
    pin->proxy()->setParentItem(this);
    pin->proxy()->setWidget(pin);
    refreshNode();
}

void GNode::deletePin(Pin *pin)
{
    auto removeFromList = [&](Pin *pin, std::vector<Pin *> &list) {
        auto it = find(list.begin(), list.end(), pin);
        list.erase(it);
        pin->deleteLater();
    };
    if (pin->isInput())
        removeFromList(pin, _inputPins);
    else
        removeFromList(pin, _outputPins);
}

void GNode::deleteValuePins(void)
{

    auto deleteList = [&](std::vector<Pin *> &list) {
        Pin *pin;
        auto it = list.begin();

        while (it != list.end()) {
            pin = *it;
            if (pin->type() != PinProperty::Type::Exec) {
                it = list.erase(it);
                pin->deleteLater();
            }
            else
                it++;
        }
    };

    deleteList(_inputPins);
    deleteList(_outputPins);
}

int GNode::pinIndex(Pin *pin)
{
    if (pin->isInput())
        return PzaUtils::IndexInVector<Pin *>(_inputPins, pin);
    else
        return PzaUtils::IndexInVector<Pin *>(_outputPins, pin);
}

void GNode::replacePin(Pin *oldPin, Pin *newPin)
{
    std::vector<Pin *> oldLinkedPins = oldPin->linkedPins();
    deletePin(oldPin);
    for (const auto pin : oldLinkedPins) {
        Pin::CreateLink(newPin, pin);
    }
}

void GNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint click = mapFromScene(event->scenePos()).toPoint();
    bool wasSelected = isSelected();

   QGraphicsItem::mousePressEvent(event);

    _prevPos = pos();

    setOnTop();

    if (isSelected() && wasSelected == false) {
        selected();
    }

    forEachPin([&](Pin *pin) {
        if (pin->isInPlugArea(click)) {
            if (pin->linked() == false || pin->isOutput())
                Pin::CreateLink(pin, mapToScene(click));
            else
                pin->disconnectLink(mapToScene(pin->plugCenter()));
            return ;
        }
    });
}

void GNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    setOnTop();

    updateLinks();
}

void GNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    _prevPos = pos();
}

void GNode::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsItem::keyReleaseEvent(event);
    
    switch (event->key()) {
        case Qt::Key_Escape:
        {
            setPos(_prevPos);
            if (scene()->mouseGrabberItem() == this)
                ungrabMouse();
            static_cast<NoderGraphicsView *>(_scene->views().first())->setMoveCanceled(true);
        }
        default:
            break;
    }
}

void GNode::setOnTop(void)
{
    QList<QGraphicsItem *> hover = collidingItems();
    
    for (auto const &item: hover) {
        if (dynamic_cast<GNode *>(item))
            item->setZValue(0);
    }
    setZValue(1);
}

bool GNode::isInPlugzone(const QPoint &pos)
{
    return (pos.x() <= _plugzone || pos.x() >= _nodebox.width() - _plugzone);
}

void GNode::updateLinks()
{
    forEachPin([](Pin *pin) {
        if (pin->linked())
            pin->moveLinks();
    });
}

void GNode::setPinPlugzone(Pin *pin, const QPoint &origin)
{
    QPoint pos;

    if (pin->isInput())
        pos.setX(0);
    else
        pos.setX(_nodebox.width() - _plugzone);

    pos.setY(origin.y() + pin->size().height() / 2 - _plugzone / 2);
    pin->setPlugzone(QRect(pos.x(), pos.y(), _plugzone, _plugzone));
}

void GNode::drawValuePlug(QPainter *painter, PinValue *pin)
{
    QSvgRenderer svgr;

    if (pin->linked())
        svgr.load(Noder::PlugValue(pin->valueType(), true));
    else
        svgr.load(Noder::PlugValue(pin->valueType(), false));
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::drawExecPlug(QPainter *painter, PinExec *pin)
{
    QSvgRenderer svgr;

     if (pin->linked())
        svgr.load(Noder::PlugExec(true));
    else
        svgr.load(Noder::PlugExec(false));
    svgr.render(painter, pin->plugzone());
}

void GNode::drawArrayPlug(QPainter *painter, PinDecl::Array *pin)
{
    QSvgRenderer svgr;

    if (pin->linked())
        svgr.load(Noder::PlugArray(pin->valueType(), true));
    else
        svgr.load(Noder::PlugArray(pin->valueType(), false));
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);

    updateLinks();
}

void GNode::process(void)
{
    _propBoxColor->setColor(_boxColor);

    forEachInputPin([&](Pin *pin) {
        if (pin->linked() && pin->type() != PinProperty::Type::Exec) {
            pin->forEachLinkedNode([](GNode *node) {
                node->process();
            });
        }
    });

    exec();
}