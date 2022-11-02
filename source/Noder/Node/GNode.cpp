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

    color = Noder::Get().plugColor(type);
    color = Noder::Get().plugColor(type);
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

void GNode::forEachInputPin(const std::function<void(Pin *pin)> &func)
{
    for (auto pin: _inputPins) {
        func(pin);
    }
}

void GNode::forEachOutputPin(const std::function<void(Pin *pin)> &func)
{
    for (auto pin: _outputPins) {
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

Pin *GNode::addPinFromType(PinProperty::Type type, const QString &name, PinProperty::Direction direction, int index)
{
    Pin *pin = nullptr;

    switch (type) {
        case PinProperty::Type::Bool:       pin = addPin<PinDecl::Bool>(name, direction, index); break ;
        case PinProperty::Type::Float:      pin = addPin<PinDecl::Float>(name, direction, index); break ;
        case PinProperty::Type::Int:        pin = addPin<PinDecl::Int>(name, direction, index); break ;
        case PinProperty::Type::String:     pin = addPin<PinDecl::String>(name, direction, index); break ;
        case PinProperty::Type::Wildcard:   pin = addPin<PinDecl::Wildcard>(name, direction, index); break ;
        case PinProperty::Type::Enum:       pin = addPin<PinDecl::Enum>(name, direction, index); break ;
        case PinProperty::Type::Array:      pin = addPin<PinDecl::Array>(name, direction, index); break ;
        case PinProperty::Type::Interface:  pin = addPin<PinDecl::Interface>(name, direction, index); break ;
        default:                            pin = nullptr; break ;
    }
    return pin;
}

void GNode::createProxyWidget(Pin *pin)
{
    PzaLabel *label;

    pin->setProxy(new QGraphicsProxyWidget(this));
    pin->setGrid(new QGridLayout(pin));
    pin->grid()->setContentsMargins(0, 0, 0, 0);
    pin->grid()->setHorizontalSpacing(5);
    pin->grid()->setVerticalSpacing(0);

    label = new PzaLabel(pin->name(), pin);
    if (_type == NodeProperty::Type::Instance) {
        label->setProperty("pzalabel", "instance");
        QFont font = label->font();
        font.setItalic(true);
        label->setFont(font);
    }
    connect(pin, &Pin::nameChanged, label, [&, label, pin](const QString &name) {
        label->setText(name);
        pin->adjustSize();
        refreshNode();
    });
    
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if (pin->isOutput())
        pin->grid()->addWidget(label, 0, 0, Qt::AlignRight);
    else
        pin->grid()->addWidget(label, 0, 0, Qt::AlignLeft);

    if (pin->isInput()) {
        switch (pin->type()) {
            case PinProperty::Type::Bool:
            {
                PzaCheckBox *checkbox = new PzaCheckBox(pin);
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
                PzaSpinBox *valuebox = new PzaSpinBox(pin);
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
                PzaDoubleSpinBox *valuebox = new PzaDoubleSpinBox(pin);
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
                PzaLineEdit *txtbox = new PzaLineEdit(pin);
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
                break ;
        }
    }
    pin->proxy()->setWidget(pin);
    refreshNode();
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
    const QColor &color = Noder::Get().plugColor(static_cast<PinDecl::Array *>(pin)->elemType());
    xml.replace("fill:#ffffff", "fill:" + color.name());

    svgr.load(xml.toUtf8());
    svgr.render(painter, pin->plugzoneIcon());
}

void GNode::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);

    updateLinks();
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