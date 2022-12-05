#include <GNodeInstance.hpp>

GNodeInstance::GNodeInstance()
    : GNode("Instance")
{
    _pinBoxOffsetY = 5;
    _boxRadius = 20;

    _propType = _propTable->addRow<PzaLabel>("Type");
    _propBoxColor = _propTable->addRow<PzaColorBox>("Box color");
    connect(_propBoxColor, &PzaColorBox::colorChanged, this, &GNode::setColor);
    
    setType(NodeProperty::Type::Instance);
}

QVariant GNodeInstance::itemChange(GraphicsItemChange change, const QVariant &value)
{
    refreshNode();
    return QGraphicsItem::itemChange(change, value);
}

void GNodeInstance::pinBoxSize(void)
{
    forEachOutputPin([&](Pin *pin) {
        _pinBox.setWidth(pin->sizeHint().width());
        _pinBox.setHeight(std::max(pin->sizeHint().height(), _entryMiny));
        return ;
    });
}

void GNodeInstance::resizeBoxes(void)
{
    _nodebox.setWidth(_pinBox.width() + _plugzone * 2);
    _nodebox.setHeight(_pinBox.height() + _pinBoxOffsetY * 2);

    QSize size(0, 0);
    forEachOutputPin([&](Pin *pin) {
        size.setWidth(_pinBox.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entryMiny));
        pin->setSize(size);
    });
}

void GNodeInstance::setWidgetSize(void)
{
    QSize size(0, 0);
    
    forEachOutputPin([&](Pin *pin) {
        size.setWidth(_pinBox.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entryMiny));
        pin->setSize(size);
        return ;
    });
}

void GNodeInstance::positionEntries(void)
{
    QPoint pos;

    pos.setY(_pinBoxOffsetY);
    pos.setX(_plugzone);

    forEachOutputPin([&](Pin *pin) {
        if (pin->proxy()) {
            setPinPlugzone(pin, pos);
            pin->proxy()->setGeometry(QRect(pos, pin->size()));
            pin->setPos(pos);
            pin->setScenePos(mapFromScene(pos));
        }
        return ;
    });
}

void GNodeInstance::drawBoxes(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    
    painter->setBrush(_boxColor);
    painter->drawRoundedRect(_nodebox, _boxRadius, _boxRadius);
    painter->setPen(Qt::NoPen);
    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen("#00E7FF"));
        painter->drawRoundedRect(_nodebox, _boxRadius, _boxRadius);
    }
    else {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen("#BBBBBB"));
        painter->drawRoundedRect(_nodebox, _boxRadius, _boxRadius);
    }
}

void GNodeInstance::refreshNode(void)
{
    prepareGeometryChange();
    
    pinBoxSize();
    resizeBoxes();
    setWidgetSize();
    positionEntries();
    
    updateLinks();
    update();
}

void GNodeInstance::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    (void)option;
    (void)w;

    drawBoxes(painter);

    forEachOutputPin([&](Pin *pin) {
        drawVariablePlug(painter, static_cast<PinVariable *>(pin));
        return ;
    });
}