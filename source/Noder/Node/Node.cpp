#include <Node.hpp>

Node::Node(NoderScene *scene, QString name)
    : QGraphicsObject(),
    _scene(scene),
    _name(name),
    _spacing_y(5),
    _spacing_mid(10),
    _plugzone(20),
    _title_offset(20, 5),
    _plug_radius(4),
    _entry_miny(25),
    _entrybox_offsety(10),
    _entrybox(0, 0, 0, 0),
    _entrybox_in(0, 0, 0, 0),
    _entrybox_out(0, 0, 0, 0),
    _propertybox(0, 0, 0, 0),
    _titlebox(0, 0, 0, 0)
{
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setAcceptHoverEvents(true);
    setOpacity(0.9f);
    setZValue(0);

    _titlefont.setPixelSize(14);
    QFontMetrics metrics(_titlefont);
    _fontbox = metrics.boundingRect(_name);

    refreshNode();

    _scene->addItem(this);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    refreshNode();
    return QGraphicsItem::itemChange(change, value);
}

void Node::setOnTop(void)
{
    QList<QGraphicsItem *> hover = collidingItems();
    
    for (auto item: hover) {
        if (item->type() == Node::Type)
            item->setZValue(0);
    }

    setZValue(1);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint click;
    QGraphicsItem::mousePressEvent(event);

    click = mapFromScene(event->scenePos()).toPoint();

    setOnTop();

    if (!isInPlugzone(click))
        return ;

    setSelected(false);

    auto func = [click](NodeEntry *entry)
    {
        if (entry->isInPlugArea(click)) {
            if (entry->linked() == false)
                entry->createLink();
            else
                entry->disconnectLink();
            return ;
        }
    };

    forEachEntries(func);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    setOnTop();

    updateLinks();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

bool Node::isInPlugzone(QPoint pos)
{
    return (pos.x() <= _plugzone || pos.x() >= _nodebox.width() - _plugzone);
}

void Node::updateLinks()
{
    auto func = [](NodeEntry *entry)
    {
        if (entry->linked()) {
            entry->moveLink();
        }
    };

    forEachEntries(func);
}

void Node::entryboxSize()
{
    _entrybox.setWidth(0);
    _entrybox.setHeight(_entrybox_offsety);

    _entrybox_in.setWidth(0);
    _entrybox_in.setHeight(0);
    for (auto entry: _entries_in) {
        _entrybox_in.setWidth(std::max(_entrybox_in.width(), entry->sizeHint().width()));
        _entrybox_in.setHeight(_entrybox_in.height() + std::max(entry->sizeHint().height(), _entry_miny) + _spacing_y);
    }

    _entrybox_out.setWidth(0);
    _entrybox_out.setHeight(0);
    for (auto entry: _entries_out) {
        _entrybox_out.setWidth(std::max(_entrybox_out.width(), entry->sizeHint().width()));
        _entrybox_out.setHeight(_entrybox_out.height() + std::max(entry->sizeHint().height(), _entry_miny) + _spacing_y);
    }

    if (needSpacing())
        _entrybox.setWidth(_entrybox_in.width() + _entrybox_out.width() + _spacing_mid);
    else
        _entrybox.setWidth(_entrybox_in.width() + _entrybox_out.width());

    if (_entrybox_in.height() < _entrybox_out.height())
        _entrybox.setHeight(_entrybox_out.height());
    else
        _entrybox.setHeight(_entrybox_in.height());
}

void Node::propertyboxSize()
{
    _propertybox.setWidth(0);
    _propertybox.setHeight(0);
    for (auto property: _properties) {
        _propertybox.setWidth(std::max(_propertybox.width(), property->sizeHint().width()));
        _propertybox.setHeight(_propertybox.height() + std::max(property->sizeHint().height(), _entry_miny) + _spacing_y);
    }

    if (_propertybox.width())
        _propertybox.setWidth(_propertybox.width() + _plugzone * 2);
}

void Node::titleboxSize()
{
    _titlebox.setWidth(_fontbox.width());
    _titlebox.setHeight(_fontbox.height() + _title_offset.height() * 2);

    _fontpos.setX(_title_offset.width());
    _fontpos.setY(-_fontbox.y() + _titlebox.height() / 2 - _fontbox.height() / 2);
}

void Node::resizeBoxes()
{
    QSize max(0, 0);

    max.setWidth(std::max(max.width(), _titlebox.width()));
    max.setWidth(std::max(max.width(), _propertybox.width()));
    max.setWidth(std::max(max.width(), _entrybox.width()));

    _titlebox.setWidth(max.width() + _plugzone * 2);
    _propertybox.setWidth(max.width());
    _entrybox.setWidth(max.width());

    if (needSpacing())
        _entrybox_in.setWidth(_entrybox.width() - _entrybox_out.width() - _spacing_mid);
    else
        _entrybox_in.setWidth(_entrybox.width() - _entrybox_out.width());

    _nodebox.setWidth(max.width() + _plugzone * 2);
    _nodebox.setHeight(_titlebox.height() + _propertybox.height() + _entrybox.height() + _entrybox_offsety);
}

void Node::positionEntries(void)
{
    QPoint origin;
    QPoint pos;
    int posMaxY = 0;

    origin.setY(_titlebox.height() + _entrybox_offsety);
    origin.setX(_plugzone);
    pos = origin;

    
    origin = pos;

    for (auto entry: _entries_in) {
        if (entry->proxy()) {
            setEntryPlugzoneLeft(entry, pos);
            entry->proxy()->setGeometry(QRect(pos, entry->size()));
            pos.setY(pos.y() + entry->size().height() + _spacing_y);
            posMaxY = std::max(posMaxY, pos.y());
        }
    }

    pos = origin;
    pos.setX(_plugzone + _entrybox_in.width());
    if (needSpacing())
        pos.setX(pos.x() + _spacing_mid);

    for (auto entry: _entries_out) {
        if (entry->proxy()) {
            setEntryPlugzoneRight(entry, pos);
            entry->proxy()->setGeometry(QRect(pos, entry->size()));
            pos.setY(pos.y() + entry->size().height() + _spacing_y);
            posMaxY = std::max(posMaxY, pos.y());
        }
    }

    pos.setY(posMaxY);
    pos.setX(_plugzone);

    forEachProperties([&](NodeProperty *property){
        if (property->proxy()) {
            property->proxy()->setGeometry(QRect(pos, property->size()));
            pos.setY(pos.y() + property->size().height() + _spacing_y);
        }
    });
}

void Node::setEntryPlugzoneLeft(NodeEntry *entry, QPoint origin)
{
    QPoint pos;

    pos.setX(0);
    pos.setY(origin.y() + entry->size().height() / 2 - _plugzone / 2);
    entry->setPlugzone(QRect(pos.x(), pos.y(), _plugzone, _plugzone));
}

void Node::setEntryPlugzoneRight(NodeEntry *entry, QPoint origin)
{
    QPoint pos;

    pos.setX(_nodebox.width() - _plugzone);
    pos.setY(origin.y() + entry->size().height() / 2 - _plugzone / 2);
    entry->setPlugzone(QRect(pos.x(), pos.y(), _plugzone, _plugzone));
}

void Node::refreshNode(void)
{
    prepareGeometryChange();
    
    titleboxSize();
    entryboxSize();
    propertyboxSize();
    resizeBoxes();
    setWidgetSize();
    positionEntries();
    updateLinks();

    update();
}

void Node::drawPlugs(QPainter *painter)
{
    forEachEntries([painter](NodeEntry *entry)
    {
        entry->drawPlug(painter);
    });
}

void Node::setWidgetSize(void)
{
    QSize size(0, 0);
    
    for (auto property: _properties) {
        size.setWidth(_propertybox.width());
        size.setHeight(std::max(property->sizeHint().height(), _entry_miny));
        property->setSize(size);
    }
    for (auto entry: _entries_in) {
        size.setWidth(_entrybox_in.width());
        size.setHeight(std::max(entry->sizeHint().height(), _entry_miny));
        entry->setSize(size);
    }
    for (auto entry: _entries_out) {
        size.setWidth(_entrybox_out.width());
        size.setHeight(std::max(entry->sizeHint().height(), _entry_miny));
        entry->setSize(size);
    }
}

void Node::drawBoxes(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    
    painter->setBrush(QColor("#373737"));
    painter->drawRoundedRect(_nodebox, 3, 3);

    painter->setBrush(_titlecolor);
    painter->drawRoundedRect(_titlebox, 3, 3);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen("#00E7FF"));
        painter->drawRoundedRect(_nodebox, 3, 3);
    }
}

void Node::drawTitle(QPainter *painter)
{
    painter->setPen(QPen(QColor("#DEDEDE"), 2.0));
    painter->setFont(_titlefont);
    painter->drawText(_fontpos, _name);
}

void Node::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    refreshNode();

    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter->setRenderHint(QPainter::LosslessImageRendering,true);

    drawBoxes(painter);
    drawTitle(painter);
    drawPlugs(painter);
}

Node::~Node()
{
    forEachProperties([](NodeProperty *property)
    {
        property->deleteLater();
    });

    forEachEntries([](NodeEntry *entry)
    {
        entry->deleteLater();
    });
}