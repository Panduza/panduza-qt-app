
#include <Pin.hpp>
#include <GNode.hpp>
#include <Link.hpp>

using namespace PinDecl;

Pin::Pin()
{
    setStyleSheet("background-color: transparent");

    
}

void Pin::setName(const QString &name)
{
    _name = name;
    _label->setText(name);
}

void Pin::hideWidgets(void)
{
    QWidget *w;

    for (int i = 1; i < _grid->count(); i++) {
        w = _grid->itemAt(i)->widget();
        w->hide();
    }
    adjustSize();
}

void Pin::showWidgets()
{
    QWidget *w;

    for (int i = 1; i < _grid->count(); i++) {
        w = _grid->itemAt(i)->widget();
        w->show();
    }
    adjustSize();
}

void Pin::mousePressEvent(QMouseEvent *event)
{
    // Clicking on a pin should send a signal to the node (for select ect..).
    // We need to catch a press event and send an ignore event to let the node receive this signal.
    // If we click an a widget that supports it, like a button, the event will be catched.
    // But, if the click happens on a label for example, we should notify the node.
    event->ignore();
}

void Pin::CreateLink(Pin *from)
{
    Link *link;

    link = new Link(from);
    link->grabMouse();
    link->grabKeyboard();
}

void Pin::CreateLink(Pin *from, Pin *to)
{
    Link *link;

    if (from->isCompatible(to)) {
        link = new Link(from, to);
        from->connectLink(link);
        to->connectLink(link);
    }
}

void Pin::CreateLink(Pin *from, const QPointF &pos)
{
    Link *link;

    link = new Link(from);
    link->moveDrag(link->mapFromScene(pos));
    link->grabMouse();
    link->grabKeyboard();
}

void Pin::connectLink(Link *link)
{
    GNode *otherNode = nullptr;
    Pin *otherPin = nullptr;

    otherPin = link->oppositePin(this);
    otherNode = otherPin->node();

    if (!PzaUtils::IsInVector<GNode *>(_linkedNodes, otherNode))
        _linkedNodes.push_back(otherNode);
    _linkedPins.push_back(otherPin);
    _links.push_back(link);

    hideWidgets();

    onEventConnect();
    _node->onEventConnect();
    _node->refreshNode();
}

void Pin::removeLink(Link *link)
{
    GNode *otherNode = nullptr;
    Pin *otherPin = nullptr;
    
    otherPin = link->oppositePin(this);
    if (!otherPin)
        return ;
    otherNode = otherPin->node();

    PzaUtils::DeleteFromVector<Link *>(_links, link);
    PzaUtils::DeleteFromVector<GNode *>(_linkedNodes, otherNode);
    PzaUtils::DeleteFromVector<Pin *>(_linkedPins, otherPin);

    if (!linked())
        showWidgets();

    onEventDisconnect();
    _node->refreshNode();
    _node->onEventDisconnect();
}

void Pin::removeLinks(void)
{
    forEachLink([](Link *link) {
        link->deleteLater();
    });
}

void Pin::disconnectLink(const QPointF &pos)
{
    Link *link;
    Pin *from;

    link = _links.front();

    from = link->oppositePin(this);
    link->deleteLater();

    CreateLink(from, pos);
}

void Pin::moveLinks(void)
{
    forEachLink([&](Link *link) {
        link->move();
    });
}

void Pin::forEachLinkedNode(const std::function<void(GNode *Node)> &func)
{
    Pin *target;

    forEachLink([&](Link *link){
        target = link->oppositePin(this);
        func(target->node());
    });
}

void Pin::forEachLinkedPin(const std::function<void(Pin *pin)> &func)
{
    Pin *target;

    forEachLink([&](Link *link){
        target = link->oppositePin(this);
        func(target);
    });
}

void Pin::forEachLink(const std::function<void(Link *link)> &func)
{
    for (auto const &link: _links) {
        func(link);
    }
}

void Pin::setupProxyWidget(void)
{
    _proxy = new QGraphicsProxyWidget();
    _grid = new QGridLayout(this);

    _grid->setContentsMargins(0, 0, 0, 0);
    _grid->setHorizontalSpacing(5);
    _grid->setVerticalSpacing(0);

    _label = new PzaLabel(this);
    _label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (isInput())
        _grid->addWidget(_label, 0, 0, Qt::AlignLeft);
    else
        _grid->addWidget(_label, 0, 0, Qt::AlignRight);
}

Pin::~Pin()
{
    _dead = true;
    forEachLink([&](Link *link) {
        link->deleteLater();
    });
}

PinValue::PinValue()
{
    _type = PinProperty::Type::Value;
}

bool PinValue::isCompatible(Pin *to)
{
    if (to->type() == _type)
        return PzaUtils::IsInVector<NoderVar::Type>(compatibles(), static_cast<PinValue *>(to)->valueType());
    return false;
}

PinValue *PinValue::PinTypeToObj(const NoderVar::Type type)
{
    switch (type) {
        case NoderVar::Type::Bool:       return new PinDecl::Bool();
        case NoderVar::Type::Float:      return new PinDecl::Float();
        case NoderVar::Type::Int:        return new PinDecl::Int();
        case NoderVar::Type::String:     return new PinDecl::String();
        case NoderVar::Type::Wildcard:   return new PinDecl::Wildcard();
        case NoderVar::Type::Enum:       return new PinDecl::Enum();
        case NoderVar::Type::Array:      return new PinDecl::Array();
        case NoderVar::Type::Interface:  return new PinDecl::Interface();
    }
    return nullptr;
}

PinExec::PinExec()
{
    _type = PinProperty::Type::Exec;
}

bool PinExec::isCompatible(Pin *to)
{
    return (to->type() == _type);
}

Float::Float()
    : PinValue()
{
    _valueType = NoderVar::Type::Float;
}

void Float::setupWidgets(void)
{
    Pin::setupWidgets();

    _box = new PzaDoubleSpinBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void Float::setValue(const int value)
{
    setValue((double)value);
}

void Float::setValue(const bool value)
{
    setValue((double)value);
}

void Float::setValue(const QString &value)
{
    bool ok;
    double res;

    res = value.toDouble(&ok);
    if (ok)
        setValue(res);
    else
        qWarning() << "Input string" << value << "cannot be converted to float.. The result will be undefined";
}

Int::Int()
    : PinValue()
{
    _valueType = NoderVar::Type::Int;
}

void Int::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaSpinBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void Int::setValue(const double value)
{
    setValue((int)value);
}

void Int::setValue(const bool value)
{
    setValue((bool)value);
}

void Int::setValue(const QString &value)
{
    bool ok;
    int res;

    res = value.toInt(&ok);
    if (ok)
        setValue(res);
    else
        qWarning() << "Input string" << value << "cannot be converted to an integer.. The result will be undefined";
}

Bool::Bool()
    : PinValue()
{
    _valueType = NoderVar::Type::Bool;
}

void Bool::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaCheckBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _grid->setColumnStretch(1, 1);
    _grid->addWidget(_box, 0, 1);
}

void Bool::setValue(const double value)
{
    setValue((bool)value);
}

void Bool::setValue(const int value)
{
    setValue((bool)value);
}

void Bool::setValue(const QString &value)
{
    const QString &str = value.toLower().trimmed();

    if (str == "true")
        setValue(true);
    else
        setValue(false);
}

String::String()
    : PinValue()
{
    _valueType = NoderVar::Type::String;
}

void String::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaLineEdit(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void String::setValue(const int value)
{
    setValue(QString::number(value));
}

void String::setValue(const double value)
{
    setValue(QString::number(value));
}

void String::setValue(const bool value)
{
    (value) ? setValue("true") : setValue("false");
}

Enum::Enum()
    : PinValue()
{
    _valueType = NoderVar::Type::Enum;
}

void Enum::setupWidgets(void)
{
    Pin::setupWidgets();
}

void Enum::modifyEnumName(const QString &name)
{
    initialize(name);
    forEachLink([&](Link *link) {
        Pin *opposite = link->oppositePin(this);
        if (!isCompatible(opposite))
            link->deleteLater();            
    });
}

void Enum::initialize(const QString &name)
{
    _list = Noder::Get().enumValues(name);
    _enumName = name;
    initialized();
}

bool Enum::isCompatible(Pin *to)
{
    PinDecl::Enum *toEnum;

    if (!PinValue::isCompatible(to))
        return false;

    toEnum = static_cast<PinDecl::Enum *>(to);
    return (enumName() == toEnum->enumName());
}

Array::Array()
    : PinValue()
{
    _valueType = NoderVar::Type::Array;
}

bool Array::isCompatible(Pin *to)
{
    PinValue *elem;
    PinValue *toElem;
    PinDecl::Array *toArray;
    bool ret = false;

    if (!PinValue::isCompatible(to))
        return false;

    toArray = static_cast<PinDecl::Array *>(to);
    elem = PinValue::PinTypeToObj(elemType());
    toElem = PinValue::PinTypeToObj(toArray->elemType());

    ret = elem->isCompatible(toElem);

    elem->deleteLater();
    toElem->deleteLater();

    return ret;
}

void Array::onEventConnect(void)
{
    PinDecl::Array *target;

    target = dynamic_cast<PinDecl::Array *>(linkedPins().front());
    if (target == nullptr || target->elemType() == NoderVar::Type::Wildcard)
        return ;
    setElemType(target->elemType());

    _node->onEventConnect();
}

void Array::onEventDisconnect(void)
{
    if (isInput()) {
        setElemType(_oriElemType);
        _node->onEventDisconnect();
    }
}

Interface::Interface()
    : PinValue()
{
    _valueType = NoderVar::Type::Interface;
}

bool PinDecl::Interface::isCompatible(Pin *to)
{
    // todo : check if interface is compatible (io->io / ! psu->io)
    (void)to;
    return false;
}