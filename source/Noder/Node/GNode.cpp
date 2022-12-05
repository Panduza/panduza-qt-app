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

    _propTable = new PzaForm();
}

void GNode::setColor(const QColor &color)
{
    _boxColor = color;
    refreshNode();
}

void GNode::setType(NodeProperty::Type type)
{
    _type = type;
    _propType->setText(Noder::Get().NodeTypeName(_type));
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

Pin *GNode::addPin(const QString &name, const PinProperty &pinProp, const NoderVarProps &varProps, int index)
{
    Pin *ret = nullptr;

    switch (pinProp.type) {
        case PinProperty::Type::Exec:
            ret = addExec(name, pinProp.direction, index);
            break;
        case PinProperty::Type::Variable:
            ret = addVariable(name, pinProp.direction, varProps, index);
            if (ret)
                static_cast<PinVariable *>(ret)->setSubType(varProps.subType);
            break;
    }
    return ret;
}

PinDecl::Exec *GNode::addExec(const QString &name, const PinProperty::Direction direction, int index)
{
    return addPin<PinDecl::Exec>(name, direction, index);
}

PinDecl::Exec *GNode::addExecInput(const QString &name, int index)
{
    return addExec(name, PinProperty::Direction::Input, index);
}

PinDecl::Exec *GNode::addExecOutput(const QString &name, int index)
{
    return addExec(name, PinProperty::Direction::Output, index);
}

PinVariable *GNode::addVariable(const QString &name, const PinProperty::Direction direction, const NoderVarProps &varProps, int index)
{
    PinVariable *ret = nullptr;

    switch (varProps.container) {
        case NoderVarProps::Container::Reference:
            ret = addRef(name, direction, varProps.type, varProps.subType, index);
            break;
        case NoderVarProps::Container::Array:
            ret = addArray(name, direction, varProps.type, varProps.subType, index);
            break;
    }
    return ret;
}

PinRef *GNode::addRef(const QString &name, const PinProperty::Direction dir, const NoderVarProps::Type type, const QString &subType, int index)
{
    PinRef *ret = nullptr;

    switch (type) {
        case NoderVarProps::Type::Bool:
            ret = addPin<PinDecl::Ref::Bool>(name, dir, index);
            break;
        case NoderVarProps::Type::Int:
            ret = addPin<PinDecl::Ref::Int>(name, dir, index);
            break;
        case NoderVarProps::Type::Float:
            ret = addPin<PinDecl::Ref::Float>(name, dir, index);
            break;
        case NoderVarProps::Type::String:
            ret = addPin<PinDecl::Ref::String>(name, dir, index);
            break;
        case NoderVarProps::Type::Enum:
            ret = addPin<PinDecl::Ref::Enum>(name, dir, index);
            break;
        case NoderVarProps::Type::Interface:
            ret = addPin<PinDecl::Ref::Interface>(name, dir, index);
            break;
        case NoderVarProps::Type::Wildcard:
            ret = addPin<PinDecl::Ref::Wildcard>(name, dir, index);
            break;
    }
    ret->setSubType(subType);
    return ret;
}

PinRef *GNode::addRefInput(const QString &name, const NoderVarProps::Type type, const QString &subType, int index)
{
    return addRef(name, PinProperty::Direction::Input, type, subType, index);
}

PinRef *GNode::addRefOutput(const QString &name, const NoderVarProps::Type type, const QString &subType, int index)
{
    return addRef(name, PinProperty::Direction::Output, type, subType, index);
}

PinArray *GNode::addArray(const QString &name, const PinProperty::Direction dir, const NoderVarProps::Type type, const QString &subType, int index)
{
    PinArray *ret = nullptr;

    switch (type) {
        case NoderVarProps::Type::Bool:
            ret = addPin<PinDecl::Array::Bool>(name, dir, index);
            break;
        case NoderVarProps::Type::Int:
            ret = addPin<PinDecl::Array::Int>(name, dir, index);
            break;
        case NoderVarProps::Type::Float:
            ret = addPin<PinDecl::Array::Float>(name, dir, index);
            break;
        case NoderVarProps::Type::String:
            ret = addPin<PinDecl::Array::String>(name, dir, index);
            break;
        case NoderVarProps::Type::Enum:
            ret = addPin<PinDecl::Array::Enum>(name, dir, index);
            break;
        case NoderVarProps::Type::Interface:
            ret = addPin<PinDecl::Array::Interface>(name, dir, index);
            break;
        case NoderVarProps::Type::Wildcard:
            ret = addPin<PinDecl::Array::Wildcard>(name, dir, index);
            break;
        default:
            break;
    }
    if (ret)
        ret->setSubType(subType);
    return ret;
}

PinArray *GNode::addArrayInput(const QString &name, const NoderVarProps::Type type, const QString &subType, int index)
{
    return addArray(name, PinProperty::Direction::Input, type, subType, index);
}

PinArray *GNode::addArrayOutput(const QString &name, const NoderVarProps::Type type, const QString &subType, int index)
{
    return addArray(name, PinProperty::Direction::Output, type, subType, index);
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
        PzaUtils::DeleteFromVector<Pin *>(list, pin);
        pin->kill();
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
                pin->kill();
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

void GNode::drawVariablePlug(QPainter *painter, PinVariable *pin)
{
    switch (pin->varProps().container) {
        case NoderVarProps::Container::Reference:
            drawRefPlug(painter, static_cast<PinRef *>(pin));
            break;
        case NoderVarProps::Container::Array:
            drawArrayPlug(painter, static_cast<PinArray *>(pin));
            break;
    }
}

void GNode::drawRefPlug(QPainter *painter, PinRef *pin)
{
    QSvgRenderer svgr;

    if (pin->linked())
        svgr.load(Noder::PlugValue(pin->varProps().type, true));
    else
        svgr.load(Noder::PlugValue(pin->varProps().type, false));
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::drawExecPlug(QPainter *painter, PinDecl::Exec *pin)
{
    QSvgRenderer svgr;

     if (pin->linked())
        svgr.load(Noder::PlugExec(true));
    else
        svgr.load(Noder::PlugExec(false));
    svgr.render(painter, pin->plugzone());
}

void GNode::drawArrayPlug(QPainter *painter, PinArray *pin)
{
    QSvgRenderer svgr;

    if (pin->linked())
        svgr.load(Noder::PlugArray(pin->varProps().type, true));
    else
        svgr.load(Noder::PlugArray(pin->varProps().type, false));
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

void GNode::kill(void)
{
    PzaUtils::ForEachDeleteInVector<Pin *>(_inputPins, [&](Pin *pin) {
        pin->kill();
    });
    PzaUtils::ForEachDeleteInVector<Pin *>(_outputPins, [&](Pin *pin) {
        pin->kill();
    });
    deleteLater();
}