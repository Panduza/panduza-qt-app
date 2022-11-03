#include <GNodeBasic.hpp>

#include <Noder.hpp>
#include <NoderScene.hpp>
#include <Link.hpp>

GNodeBasic::GNodeBasic(const QString &name)
    : GNode(name),
    _spacingY(2),
    _spacingMid(10),
    _pinBoxIn(0, 0, 0, 0)
{
    _title = new struct title;
    _title->box = QRect(0, 0, 0, 0);
    _title->size = QSize(0, 20);
    _title->font.setPixelSize(14);
    _title->fontcolor = QColor("#DEDEDE");
    _title->offset = QSize(20, 5);

    _propUserName = _propTable->addProperty<PzaLineEdit>("Custom Name");
    _propUserName->setText(_userName);
    connect(_propUserName, &PzaLineEdit::editingFinished, this, &GNodeBasic::setUserName);
    _propName = _propTable->addProperty<PzaLabel>("Name");
    _propName->setText(_userName);
    _propType = _propTable->addProperty<PzaLabel>("Type");
    _propBoxColor = _propTable->addProperty<PzaColorBox>("Box color");
    connect(_propBoxColor, &PzaColorBox::colorChanged, this, &GNode::setColor);
    _propTitleColor = _propTable->addProperty<PzaColorBox>("Title color");
    connect(_propTitleColor, &PzaColorBox::colorChanged, this, &GNodeBasic::setTitleColor);
}

void GNodeBasic::setUserName(void)
{
    _userName = _propUserName->text();
    refreshNode();
}

void GNodeBasic::refreshUserName(const QString &name)
{
    GNode::refreshUserName(name);
    _propUserName->setText(name);
}

void GNodeBasic::setTitleColor(const QColor &color)
{
    _title->boxcolor = color;
    refreshNode();
}

void GNodeBasic::setType(NodeProperty::Type type)
{
    GNode::setType(type);
    _title->boxcolor = defaultTitleColor(_type);
}

QVariant GNodeBasic::itemChange(GraphicsItemChange change, const QVariant &value)
{
    refreshNode();
    return QGraphicsItem::itemChange(change, value);
}

const QColor &GNodeBasic::defaultTitleColor(const NodeProperty::Type &type)
{
    static std::unordered_map<NodeProperty::Type, QColor> map = {
        {NodeProperty::Type::Exec, QColor("blue")},
        {NodeProperty::Type::Operation, QColor("green")},
        {NodeProperty::Type::Branch, QColor("gray")},
        {NodeProperty::Type::Event, QColor("red")},
    };

    return map[type];
}

void GNodeBasic::addPintoMultiPin(struct multiPin *s)
{
    int index;
    Pin *pin;
    Pin *last;
    int size;

    size = s->list->size();

    if (size == s->max)
        return ;
    last = s->list->back();
    index = PzaUtils::IndexInVector<Pin *>(_inputPins, last);
    if (index == -1)
        return ;
    pin = addPinFromType(s->type, s->pinName + " " + QString::number(size), PinProperty::Direction::Input, index + 1);
    s->list->push_back(pin);
}

void GNodeBasic::createProxyMultiPin(struct multiPin *s)
{
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);
    PzaMoreLess *moreLess = new PzaMoreLess(s->name);

    s->proxy = proxy;
    s->w = moreLess;

    proxy->setWidget(s->w);

    connect(moreLess, &PzaMoreLess::more, this, [&, s]{
        addPintoMultiPin(s);
    });

    connect(moreLess, &PzaMoreLess::less, this, [&, s]{
        int index;
        Pin *last;

        if ((int)s->list->size() == s->min)
            return ;
        last = s->list->back();
        index = PzaUtils::IndexInVector<Pin *>(_inputPins, last);
        if (index == -1)
            return ;
        PzaUtils::DeleteFromVector<Pin *>(*s->list, last);

        deletePin(last);
    });

}

void GNodeBasic::forEachMultiPin(const std::function<void(struct multiPin *s)> &func)
{
    for (auto const &s: _multiPinStructs) {
        func(s);
    }
}

struct GNodeBasic::multiPin *GNodeBasic::findMultiPinFromList(std::vector<Pin *> *list)
{
    for (auto const &s: _multiPinStructs) {
        if (s->list == list)
            return s;
    }
    return nullptr;
}

void GNodeBasic::pinBoxSize()
{
    _pinBox.setWidth(0);
    _pinBox.setHeight(0);

    _pinBoxIn.setWidth(0);
    _pinBoxIn.setHeight(0);
    forEachInputPin([&](Pin *pin) {
        _pinBoxIn.setWidth(std::max(_pinBoxIn.width(), pin->sizeHint().width()));
        _pinBoxIn.setHeight(_pinBoxIn.height() + std::max(pin->sizeHint().height(), _entryMiny) + _spacingY);
    });

    _pinBoxOut.setWidth(0);
    _pinBoxOut.setHeight(0);
    forEachOutputPin([&](Pin *pin) {
        _pinBoxOut.setWidth(std::max(_pinBoxOut.width(), pin->sizeHint().width()));
        _pinBoxOut.setHeight(_pinBoxOut.height() + std::max(pin->sizeHint().height(), _entryMiny) + _spacingY);
    });
    
    if (_multiPinStructs.size() > 0) {
        _pinBoxOut.setHeight(_pinBoxOut.height() + 10);
    }

    for (auto const &s: _multiPinStructs) {
        _pinBoxOut.setWidth(std::max(_pinBoxOut.width(), s->w->sizeHint().width()));
        _pinBoxOut.setHeight(_pinBoxOut.height() + std::max(s->w->sizeHint().height(), _entryMiny) + _spacingY);
    }

    if (needSpacing())
        _pinBox.setWidth(_pinBoxIn.width() + _pinBoxOut.width() + _spacingMid);
    else
        _pinBox.setWidth(_pinBoxIn.width() + _pinBoxOut.width());

    if (_pinBoxIn.height() < _pinBoxOut.height())
        _pinBox.setHeight(_pinBoxOut.height());
    else
        _pinBox.setHeight(_pinBoxIn.height());
}

void GNodeBasic::titleboxSize()
{
    _title->fontbox = QFontMetrics(_title->font).boundingRect(_userName);
    _title->box.setWidth(_title->fontbox.width());
    _title->box.setHeight(_title->fontbox.height() + _title->offset.height() * 2);

    _title->fontpos.setX(_title->offset.width());
    _title->fontpos.setY(-_title->fontbox.y() + _title->box.height() / 2 - _title->fontbox.height() / 2);
}

void GNodeBasic::resizeBoxes()
{
    QSize max(0, 0);

    max.setWidth(std::max(max.width(), _title->box.width()));
    max.setWidth(std::max(max.width(), _pinBox.width()));

    _title->box.setWidth(max.width() + _plugzone * 2);
    _pinBox.setWidth(max.width());

    if (needSpacing())
        _pinBoxIn.setWidth(_pinBox.width() - _pinBoxOut.width() - _spacingMid);
    else
        _pinBoxIn.setWidth(_pinBox.width() - _pinBoxOut.width());

    _nodebox.setWidth(max.width() + _plugzone * 2);
    _nodebox.setHeight(_title->box.height() + _pinBox.height() + _pinBoxOffsetY);
}

void GNodeBasic::positionEntries(void)
{
    QPoint origin;
    QPoint pos;
    int posMaxY = 0;

    origin.setY(_title->box.height() + _pinBoxOffsetY);
    origin.setX(_plugzone);
    pos = origin;

    origin = pos;

    forEachInputPin([&](Pin *pin) {
        if (pin->proxy()) {
            setPinPlugzone(pin, pos);
            pin->proxy()->setGeometry(QRect(pos, pin->size()));
            pin->setPos(pos);
            pin->setScenePos(mapFromScene(pos));
            pos.setY(pos.y() + pin->size().height() + _spacingY);
            posMaxY = std::max(posMaxY, pos.y());
        }
    });

    pos = origin;
    pos.setX(_plugzone + _pinBoxIn.width());
    if (needSpacing())
        pos.setX(pos.x() + _spacingMid);

    forEachOutputPin([&](Pin *pin) {
        if (pin->proxy()) {
            setPinPlugzone(pin, pos);
            pin->proxy()->setGeometry(QRect(pos, pin->size()));
            pin->setScenePos(mapToScene(pos));
            pos.setY(pos.y() + pin->size().height() + _spacingY);
            posMaxY = std::max(posMaxY, pos.y());
        }
    });

    if (_multiPinStructs.size() > 0)
        pos.setY(pos.y());

    for (auto const &s: _multiPinStructs) {
        s->proxy->setGeometry(QRect(pos, s->w->size()));
        pos.setY(pos.y() + s->w->size().height() + _spacingY);
        posMaxY = std::max(posMaxY, pos.y());
    }
}

void GNodeBasic::refreshNode(void)
{
    prepareGeometryChange();
    
    titleboxSize();
    pinBoxSize();
    resizeBoxes();
    setWidgetSize();
    positionEntries();
    updateLinks();

    update();
}

void GNodeBasic::setWidgetSize(void)
{
    QSize size(0, 0);
    
    forEachInputPin([&](Pin *pin) {
        size.setWidth(_pinBoxIn.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entryMiny));
        pin->setSize(size);
    });
    forEachOutputPin([&](Pin *pin) {
        size.setWidth(_pinBoxOut.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entryMiny));
        pin->setSize(size);
    });
    for (auto const &s: _multiPinStructs) {
        s->w->setFixedSize(_pinBoxOut.width(), std::max(s->w->sizeHint().height(), _entryMiny));
    }
}

void GNodeBasic::drawBoxes(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    
    painter->setBrush(_boxColor);
    painter->drawRoundedRect(_nodebox, _boxRadius, _boxRadius);
    painter->setPen(Qt::NoPen);
    painter->setBrush(_title->boxcolor);
    painter->drawRoundedRect(_title->box, _boxRadius, _boxRadius);
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

void GNodeBasic::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    (void)option;
    (void)w;

    QPen pen;
    QBrush brush;

    drawBoxes(painter);
    pen.setColor(_title->fontcolor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setFont(_title->font);
    painter->drawText(_title->fontpos, _userName);

    forEachPin([&](Pin *pin) {
        switch (pin->type()) {
            case PinProperty::Type::Array:
                drawArrayPlug(painter, pin);
                break;
            case PinProperty::Type::Exec:
                drawExecPlug(painter, pin);
                break;
            default:
                drawValuePlug(painter, pin);
                break;
        }
    });
}

GNodeBasic::~GNodeBasic()
{
    delete _title;
}