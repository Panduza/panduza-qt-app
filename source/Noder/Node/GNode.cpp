#include <GNode.hpp>

#include <NoderDataBase.hpp>
#include <NoderScene.hpp>
#include <Link.hpp>

GNode::GNode(const QString &name)
    : QGraphicsObject(),
    _spacing_y(5),
    _spacing_mid(10),
    _plugzone(20),
    _plug_radius(4),
    _entry_miny(25),
    _pinBox_offsety(10),
    _pinBox(0, 0, 0, 0),
    _pinBox_in(0, 0, 0, 0),
    _pinBox_out(0, 0, 0, 0),
    _name(name)
{

}

void GNode::setup(void)
{
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setAcceptHoverEvents(true);
    setOpacity(0.9f);
    setZValue(0);

    setOnTop();

    if (_hasTitle) {
        _title = new struct title;
        _title->box = QRect(0, 0, 0, 0);
        _title->size = QSize(0, 20);
        _title->font.setPixelSize(14);
        _title->fontbox = QFontMetrics(_title->font).boundingRect(_name);
        _title->fontcolor = QColor("#DEDEDE");
        _title->offset = QSize(20, 5);
        _title->boxcolor = titleColor(_type);
    }

    if (_mapPlugFiles.empty()) {
        _mapPlugFiles[PlugType::Array] = initPlugType(PlugType::Array);
        _mapPlugFiles[PlugType::Exec] = initPlugType(PlugType::Exec);
        _mapPlugFiles[PlugType::Value] = initPlugType(PlugType::Value);

        _mapPlugColoredCIcon[PinProperty::Type::Wildcard] = loadColorCValue(PinProperty::Type::Wildcard);
        _mapPlugColoredCIcon[PinProperty::Type::Int] = loadColorCValue(PinProperty::Type::Int);
        _mapPlugColoredCIcon[PinProperty::Type::Bool] = loadColorCValue(PinProperty::Type::Bool);
        _mapPlugColoredCIcon[PinProperty::Type::Float] = loadColorCValue(PinProperty::Type::Float);
        _mapPlugColoredCIcon[PinProperty::Type::String] = loadColorCValue(PinProperty::Type::String);
        _mapPlugColoredCIcon[PinProperty::Type::Enum] = loadColorCValue(PinProperty::Type::Enum);
        _mapPlugColoredCIcon[PinProperty::Type::Interface] = loadColorCValue(PinProperty::Type::Interface);

        _mapPlugColoredNcIcon[PinProperty::Type::Wildcard] = loadColorNcValue(PinProperty::Type::Wildcard);
        _mapPlugColoredNcIcon[PinProperty::Type::Int] = loadColorNcValue(PinProperty::Type::Int);
        _mapPlugColoredNcIcon[PinProperty::Type::Bool] = loadColorNcValue(PinProperty::Type::Bool);
        _mapPlugColoredNcIcon[PinProperty::Type::Float] = loadColorNcValue(PinProperty::Type::Float);
        _mapPlugColoredNcIcon[PinProperty::Type::String] = loadColorNcValue(PinProperty::Type::String);
        _mapPlugColoredNcIcon[PinProperty::Type::Enum] = loadColorNcValue(PinProperty::Type::Enum);
        _mapPlugColoredNcIcon[PinProperty::Type::Interface] = loadColorNcValue(PinProperty::Type::Interface);
    }
}

const QColor &GNode::titleColor(const NodeProperty::Type &type)
{
    static std::unordered_map<NodeProperty::Type, QColor> map = {
        {NodeProperty::Type::Exec, QColor("blue")},
        {NodeProperty::Type::Operation, QColor("green")},
        {NodeProperty::Type::Branch, QColor("gray")},
        {NodeProperty::Type::Event, QColor("red")},
    };

    return map[type];
}

struct GNode::plugIcon GNode::initPlugType(PlugType type)
{
    struct GNode::plugIcon s;
    struct GNode::plugIconData dataNc;
    struct GNode::plugIconData dataC;

    switch (type) {
        case PlugType::Array:
            dataC.filename = ":/images/noder/plug_array.svg";
            dataNc.filename = ":/images/noder/plug_array.svg";
            break;
        case PlugType::Exec:
            dataC.filename = ":/images/noder/plug_exec_c.svg";
            dataNc.filename = ":/images/noder/plug_exec_nc.svg";
            break;
        case PlugType::Value:
            dataC.filename = ":/images/noder/plug_value_c.svg";
            dataNc.filename = ":/images/noder/plug_value_nc.svg";
            break;
    }

    dataC.data = loadPlugIcon(dataC.filename);
    dataNc.data = loadPlugIcon(dataNc.filename);

    s.plugC = dataC;
    s.plugNc = dataNc;
    
    return s;
}

QString GNode::loadPlugIcon(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open" << filename;
        return nullptr;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();
    return text;
}

QByteArray GNode::loadColorValue(const PinProperty::Type &type, bool linked)
{
    QString xml;
    QColor color;

    color = NBD_INST->plugColor(type);
    color = NBD_INST->plugColor(type);
    if (linked)
        xml = _mapPlugFiles[PlugType::Value].plugC.data;
    else
        xml = _mapPlugFiles[PlugType::Value].plugNc.data;
    xml.replace("fill:#ffffff", "fill:" + color.name());
    return xml.toUtf8();
}

QByteArray GNode::loadColorCValue(const PinProperty::Type &type)
{
    return loadColorValue(type, 1);
}

QByteArray GNode::loadColorNcValue(const PinProperty::Type &type)
{
    return loadColorValue(type, 0);

}

void GNode::forEachInputPin(std::function<void(Pin *pin)> func)
{
    for (auto pin: _inputPins) {
        func(pin);
    }
}

void GNode::forEachOutputPin(std::function<void(Pin *pin)> func)
{
    for (auto pin: _outputPins) {
        func(pin);
    }
}

void GNode::forEachPin(std::function<void(Pin *pin)> func)
{
   forEachInputPin(func);
   forEachOutputPin(func);
}

void GNode::forEachMultiPin(std::function<void(struct multiPin *s)> func)
{
    for (auto s: _multiPinStructs) {
        func(s);
    }
}

struct GNode::multiPin *GNode::findMultiPinFromList(std::vector<Pin *> *list)
{
    for (auto s: _multiPinStructs) {
        if (s->list == list)
            return s;
    }
    return nullptr;
}

void GNode::setScene(NoderScene *scene)
{
    scene->addItem(this);
    _scene = scene;
}

Pin *GNode::addPinFromType(PinProperty::Type type, const QString &name, PinProperty::Direction direction, int index)
{
    switch (type) {
        case PinProperty::Type::Bool:       return addPin<PinDecl::Bool>(name, direction, index);
        case PinProperty::Type::Float:      return addPin<PinDecl::Float>(name, direction, index);
        case PinProperty::Type::Int:        return addPin<PinDecl::Int>(name, direction, index);
        case PinProperty::Type::String:     return addPin<PinDecl::String>(name, direction, index);
        case PinProperty::Type::Wildcard:   return addPin<PinDecl::Wildcard>(name, direction, index);
        case PinProperty::Type::Enum:       return addPin<PinDecl::Enum>(name, direction, index);
        case PinProperty::Type::Array:      return addPin<PinDecl::Array>(name, direction, index);
        case PinProperty::Type::Interface:  return addPin<PinDecl::Interface>(name, direction, index);
        default:                            return nullptr;
    }
}

void GNode::createProxyWidget(Pin *pin)
{
    QLabel *label;

    if (!_hasWidgets)
        return ;

    pin->setProxy(new QGraphicsProxyWidget(this));
    pin->setGrid(new QGridLayout(pin));
    pin->grid()->setContentsMargins(0, 0, 0, 0);
    pin->grid()->setHorizontalSpacing(5);
    pin->grid()->setVerticalSpacing(0);
    
    pin->proxy()->setWidget(pin);

    label = new QLabel(pin->name(), pin);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    pin->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    if (pin->isOutput())
        pin->grid()->addWidget(label, 0, 0, Qt::AlignRight);
    else
        pin->grid()->addWidget(label, 0, 0, Qt::AlignLeft);

    pin->setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "}"
        "QLabel {"
        "   color: #DCDCDC;"
        "   background-color: transparent;"
        "   font: 14px;"
        "}"
        "QComboBox {"
        "   border: 1px solid gray;"
        "   border-radius: 2px;"
        "   font: 14px;"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "   padding: 3px 30px 1px 3px;"
        "}"
        "QComboBox::drop-down {"
            "subcontrol-origin: padding;"
            "subcontrol-position: top right;"
            "border-top-right-radius: 3px;"
            "border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QListView {"
        "   background-color: #474747;"
        "}"
        "QDoubleSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QDoubleSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QLineEdit {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QLineEdit::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QCheckBox::indicator {"
        "   width: 14px;"
        "   height: 14px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   border-image: url(:/images/noder/checkbox/unchecked.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:unchecked:hover {"
        "   border-image: url(:/images/noder/checkbox/hover.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:checked {"
        "   border-image: url(:/images/noder/checkbox/checked.png) 0 0 0 0 stretch;"
        "}"
    );

    if (pin->isOutput())
        return ;

    switch (pin->type()) {
        case PinProperty::Type::Bool:
        {
            QCheckBox *checkbox = new QCheckBox(pin);
            checkbox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            pin->grid()->setColumnStretch(1, 1);
            pin->grid()->addWidget(checkbox, 0, 1);
            connect(pin, &Pin::askWidget, this, [pin, checkbox]() {
                pin->setValue(checkbox->isChecked());
            });
            break ;
        }
        case PinProperty::Type::Int:
        {
            QSpinBox *valuebox = new QSpinBox(pin);
            PinDecl::Int *iPin = static_cast<PinDecl::Int*>(pin);
            valuebox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            valuebox->setMinimum(iPin->min());
            valuebox->setMaximum(iPin->max());
            valuebox->setFixedWidth(100);
            pin->grid()->addWidget(valuebox, 0, 1);
            connect(pin, &Pin::askWidget, this, [pin, valuebox]() {
                pin->setValue(valuebox->value());
            });
            break ;
        }
        case PinProperty::Type::Float:
        {
            QDoubleSpinBox *valuebox = new QDoubleSpinBox(pin);
            PinDecl::Float *fPin = static_cast<PinDecl::Float*>(pin);
            valuebox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            valuebox->setMinimum(fPin->min());
            valuebox->setMaximum(fPin->max());
            valuebox->setFixedWidth(100);
            pin->grid()->addWidget(valuebox, 0, 1);
            connect(pin, &Pin::askWidget, this, [pin, valuebox]() {
                pin->setValue(valuebox->value());
            });
            break ;
        }
        case PinProperty::Type::String:
        {
            QLineEdit *txtbox = new QLineEdit(pin);
            txtbox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            txtbox->setFixedWidth(100);
            pin->grid()->addWidget(txtbox, 0, 1);
            connect(pin, &Pin::askWidget, this, [pin, txtbox]() {
                pin->setValue(txtbox->text());
            });
            break ;
        }
        case PinProperty::Type::Enum:
        {
            PzaComboBox *combo = new PzaComboBox(pin);
            combo->setFixedWidth(100);
            connect(combo, &PzaComboBox::clicked, this, [&, pin](){
                forEachInputPin([](Pin *pin)
                {
                    if (dynamic_cast<PinDecl::Enum *>(pin)) {
                        pin->proxy()->setZValue(0);
                    }
                });
                pin->proxy()->setZValue(1);
            });
            PinDecl::Enum *ePin = static_cast<PinDecl::Enum*>(pin);
            connect(ePin, &PinDecl::Enum::initialized, this, [&, ePin, combo](){
                combo->clear();
                for (auto item: ePin->list()) {
                    combo->addItem(item);
                }
            });
            pin->grid()->addWidget(combo, 0, 1);
            break ;
        }
        default:
            return ;
    }
}

void GNode::addPintoMultiPin(struct multiPin *s)
{
    int index;
    Pin *pin;
    Pin *last;
    int size;

    size = s->list->size();

    if (size == s->max)
        return ;
    last = s->list->back();
    index = PzaUtils::indexInVector<Pin *>(_inputPins, last);
    if (index == -1)
        return ;
    pin = addPinFromType(s->type, s->pinName + " " + QString::number(size), PinProperty::Direction::Input, index + 1);
    s->list->push_back(pin);
}

void GNode::createProxyMultiPin(struct multiPin *s)
{
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);
    QWidget *w = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(w);
    QPushButton *add = new QPushButton(w);
    QPushButton *rm = new QPushButton(w);
    QLabel *label = new QLabel(s->name);

    add->setObjectName("add");
    rm->setObjectName("rm");

    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    add->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    rm->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    w->setStyleSheet(
    "QWidget {"
    "   background-color: transparent;"
    "}"
    "QLabel {"
    "   color: #DCDCDC;"
    "   background-color: transparent;"
    "   font: 14px;"
    "}"
    "QPushButton {"
    "   max-width: 16px;"
    "   max-height: 16px;"
    "}"
    "QPushButton#add {"
        "   border-image: url(:/images/noder/plus.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#rm {"
        "    border-image: url(:/images/noder/minus.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#add::hover {"
        "   border-image: url(:/images/noder/plus_hover.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#rm::hover {"
        "    border-image: url(:/images/noder/minus_hover.png) 0 0 0 0 stretch;"
        "}"
    );

    layout->addStretch(1);
    layout->addWidget(label);
    layout->addWidget(add);
    layout->addWidget(rm);

    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);

    proxy->setWidget(w);
    s->proxy = proxy;
    s->w = w;

    connect(add, &QPushButton::clicked, this, [&, s]{
        addPintoMultiPin(s);
    });

    connect(rm, &QPushButton::clicked, this, [&, s]{
        int index;
        Pin *last;

        if ((int)s->list->size() == s->min)
            return ;
        last = s->list->back();
        index = PzaUtils::indexInVector<Pin *>(_inputPins, last);
        if (index == -1)
            return ;
        PzaUtils::deleteFromVector<Pin *>(*s->list, last);

        deletePin(last);
    });
}

void GNode::deletePin(Pin *pin)
{
    auto removeFromList = [&](Pin *pin, std::vector<Pin *>& list) {
        auto it = find(list.begin(), list.end(), pin);
        pin->deleteLater();
        list.erase(it);
    };
    if (pin->isInput())
        removeFromList(pin, _inputPins);
    else
        removeFromList(pin, _outputPins);
}

QVariant GNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}

void GNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint click;

    QGraphicsItem::mousePressEvent(event);

    click = mapFromScene(event->scenePos()).toPoint();

    setOnTop();

    if (!isInPlugzone(click))
        return ;

    setSelected(false);

    forEachPin([&](Pin *pin) {
        if (pin->isInPlugArea(click)) {
            if (pin->linked() == false || pin->isOutput())
                pin->createLink(pin, mapToScene(click));
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
}

void GNode::setOnTop(void)
{
    QList<QGraphicsItem *> hover = collidingItems();
    
    for (auto item: hover) {
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

void GNode::pinBoxSize()
{
    _pinBox.setWidth(0);
    _pinBox.setHeight(_pinBox_offsety);

    _pinBox_in.setWidth(0);
    _pinBox_in.setHeight(0);
    forEachInputPin([&](Pin *pin) {
        _pinBox_in.setWidth(std::max(_pinBox_in.width(), pin->sizeHint().width()));
        _pinBox_in.setHeight(_pinBox_in.height() + std::max(pin->sizeHint().height(), _entry_miny) + _spacing_y);
    });

    _pinBox_out.setWidth(0);
    _pinBox_out.setHeight(0);
    forEachOutputPin([&](Pin *pin) {
        _pinBox_out.setWidth(std::max(_pinBox_out.width(), pin->sizeHint().width()));
        _pinBox_out.setHeight(_pinBox_out.height() + std::max(pin->sizeHint().height(), _entry_miny) + _spacing_y);
    });
    
    if (_multiPinStructs.size() > 0)
        _pinBox_out.setHeight(_pinBox_out.height() + 10);

    for (auto s: _multiPinStructs) {
        _pinBox_out.setWidth(std::max(_pinBox_out.width(), s->w->sizeHint().width()));
        _pinBox_out.setHeight(_pinBox_out.height() + std::max(s->w->sizeHint().height(), _entry_miny) + _spacing_y);
    }

    if (needSpacing())
        _pinBox.setWidth(_pinBox_in.width() + _pinBox_out.width() + _spacing_mid);
    else
        _pinBox.setWidth(_pinBox_in.width() + _pinBox_out.width());

    if (_pinBox_in.height() < _pinBox_out.height())
        _pinBox.setHeight(_pinBox_out.height());
    else
        _pinBox.setHeight(_pinBox_in.height());
}

void GNode::titleboxSize()
{
    _title->box.setWidth(_title->fontbox.width());
    _title->box.setHeight(_title->fontbox.height() + _title->offset.height() * 2);

    _title->fontpos.setX(_title->offset.width());
    _title->fontpos.setY(-_title->fontbox.y() + _title->box.height() / 2 - _title->fontbox.height() / 2);
}

void GNode::resizeBoxes()
{
    QSize max(0, 0);

    max.setWidth(std::max(max.width(), _title->box.width()));
    max.setWidth(std::max(max.width(), _pinBox.width()));

    if (_hasTitle)
        _title->box.setWidth(max.width() + _plugzone * 2);
    _pinBox.setWidth(max.width());

    if (needSpacing())
        _pinBox_in.setWidth(_pinBox.width() - _pinBox_out.width() - _spacing_mid);
    else
        _pinBox_in.setWidth(_pinBox.width() - _pinBox_out.width());

    _nodebox.setWidth(max.width() + _plugzone * 2);
    if (_hasTitle)
        _nodebox.setHeight(_title->box.height() + _pinBox.height() + _pinBox_offsety);
    else
        _nodebox.setHeight(_pinBox.height() + _pinBox_offsety);
}

void GNode::positionEntries(void)
{
    QPoint origin;
    QPoint pos;
    int posMaxY = 0;

    if (_hasTitle)
        origin.setY(_title->box.height() + _pinBox_offsety);
    else
        origin.setY(_pinBox_offsety);
    origin.setX(_plugzone);
    pos = origin;

    origin = pos;

    forEachInputPin([&](Pin *pin) {
        if (pin->proxy()) {
            setPinPlugzone(pin, pos);
            pin->proxy()->setGeometry(QRect(pos, pin->size()));
            pin->setPos(pos);
            pin->setScenePos(mapFromScene(pos));
            pos.setY(pos.y() + pin->size().height() + _spacing_y);
            posMaxY = std::max(posMaxY, pos.y());
        }
    });

    pos = origin;
    pos.setX(_plugzone + _pinBox_in.width());
    if (needSpacing())
        pos.setX(pos.x() + _spacing_mid);

    forEachOutputPin([&](Pin *pin) {
        if (pin->proxy()) {
            setPinPlugzone(pin, pos);
            pin->proxy()->setGeometry(QRect(pos, pin->size()));
            pin->setScenePos(mapToScene(pos));
            pos.setY(pos.y() + pin->size().height() + _spacing_y);
            posMaxY = std::max(posMaxY, pos.y());
        }
    });

    if (_multiPinStructs.size() > 0)
        pos.setY(pos.y());

    for (auto s: _multiPinStructs) {
        s->proxy->setGeometry(QRect(pos, s->w->size()));
        pos.setY(pos.y() + s->w->size().height() + _spacing_y);
        posMaxY = std::max(posMaxY, pos.y());
    }
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

void GNode::refreshNode(void)
{
    prepareGeometryChange();
    
    if (_hasTitle)
        titleboxSize();
    pinBoxSize();
    resizeBoxes();
    setWidgetSize();
    positionEntries();

    update();
}

void GNode::setWidgetSize(void)
{
    QSize size(0, 0);
    
    forEachInputPin([&](Pin *pin) {
        size.setWidth(_pinBox_in.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entry_miny));
        pin->setSize(size);
    });
    forEachOutputPin([&](Pin *pin) {
        size.setWidth(_pinBox_out.width());
        size.setHeight(std::max(pin->sizeHint().height(), _entry_miny));
        pin->setSize(size);
    });
    for (auto s: _multiPinStructs) {
        s->w->setFixedSize(_pinBox_out.width(), std::max(s->w->sizeHint().height(), _entry_miny));
    }
}

void GNode::drawBoxes(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    
    painter->setBrush(QColor("#373737"));
    painter->drawRoundedRect(_nodebox, 3, 3);

    if (_hasTitle) {
        painter->setBrush(_title->boxcolor);
        painter->drawRoundedRect(_title->box, 3, 3);
    }
    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen("#00E7FF"));
        painter->drawRoundedRect(_nodebox, 3, 3);
    }
    else {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen("#BBBBBB"));
        painter->drawRoundedRect(_nodebox, 2, 2);
    }
}

void GNode::drawValuePlug(QPainter *painter, Pin *pin)
{
    QSvgRenderer svgr;

    if (pin->linked())
        svgr.load(_mapPlugColoredCIcon[pin->type()]);
    else
        svgr.load(_mapPlugColoredNcIcon[pin->type()]);
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::drawArrayPlug(QPainter *painter, Pin *pin)
{
    QSvgRenderer svgr;
    QString xml;

    xml = _mapPlugFiles[PlugType::Array].plugC.data;
    const QColor &color = NBD_INST->plugColor(static_cast<PinDecl::Array *>(pin)->elemType());
    xml.replace("fill:#ffffff", "fill:" + color.name());

    svgr.load(xml.toUtf8());
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::drawExecPlug(QPainter *painter, Pin *pin)
{
    QSvgRenderer svgr;

     if (pin->linked())
        svgr.load(_mapPlugFiles[PlugType::Exec].plugC.data.toUtf8());
    else
        svgr.load(_mapPlugFiles[PlugType::Exec].plugNc.data.toUtf8());
    svgr.render(painter, pin->plugzone());
}

void GNode::process(void)
{
    forEachInputPin([&](Pin *pin) {
        if (pin->linked() && pin->type() != PinProperty::Type::Exec) {
            pin->forEachLinkedNode([](GNode *node) {
                node->process();
            });
        }
    });

    exec();
}

void GNode::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *w)
{
    (void)option;
    (void)w;
    QPen pen;
    QBrush brush;

    refreshNode();

    drawBoxes(painter);
    if (_hasTitle) {
        pen.setColor(_title->fontcolor);
        pen.setWidth(2);

        painter->setPen(pen);
        painter->setFont(_title->font);
        painter->drawText(_title->fontpos, _name);
    }

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

GNode::~GNode()
{
    if (_hasTitle)
        delete _title;
}