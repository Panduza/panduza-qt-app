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
    nameChanged(_name);
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

bool Pin::isCompatible(Pin *to)
{
    return PzaUtils::IsInVector<PinProperty::Type>(compatibles(), to->type());
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
    _node->update();
    _node->updateLinks();
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

    showWidgets();

    onEventDisconnect();
    _node->refreshNode();
    _node->onEventDisconnect();
    _node->update();
    _node->updateLinks();
}

void Pin::removeLinks(void)
{
    forEachLink([](Link *link) {
        delete link;
    });
}

void Pin::disconnectLink(const QPointF &pos)
{
    Link *link;
    Pin *from;

    link = _links.at(0);

    from = link->oppositePin(this);
    delete link;

    CreateLink(from, pos);
}

void Pin::moveLinks(void)
{
    forEachLink([&](Link *link) {
        link->move();
    });
}

void Pin::forEachLinkedNode(std::function<void(GNode *Node)> func)
{
    Pin *target;

    forEachLink([&](Link *link){
        target = link->oppositePin(this);
        func(target->node());
    });
}

void Pin::forEachLinkedPin(std::function<void(Pin *pin)> func)
{
    Pin *target;

    forEachLink([&](Link *link){
        target = link->oppositePin(this);
        func(target);
    });
}

void Pin::forEachLink(std::function<void(Link *link)> func)
{
    for (auto link: _links) {
        func(link);
    }
}

Pin::~Pin()
{
    _dead = true;
    forEachLink([&](Link *link) {
        delete link;
    });
}

Wildcard::Wildcard()
{
    _type = PinProperty::Type::Wildcard;
}

Float::Float(double value, double min, double max, unsigned int decimals)
    : _value(value),
    _min(min),
    _max(max),
    _decimals(decimals)
{
    _type = PinProperty::Type::Float;
}

void Float::setValue(const int value)
{
    _value = value;
}

void Float::setValue(const bool value)
{
    _value = value;
}

void Float::setValue(const QString &value)
{
    bool ok;
    double res;

    res = value.toDouble(&ok);
    if (ok)
        _value = res;
    else
        qWarning() << "Input string" << value << "cannot be converted to float.. The result will be undefined";
}

Int::Int(int value, int min, int max)
    : _value(value),
    _min(min),
    _max(max)
{
    _type = PinProperty::Type::Int;
}

void Int::setValue(const double value)
{
    _value = value;
}

void Int::setValue(const bool value)
{
    _value = value;
}

void Int::setValue(const QString &value)
{
    bool ok;
    int res;

    res = value.toInt(&ok);
    if (ok)
        _value = res;
    else
        qWarning() << "Input string" << value << "cannot be converted to an integer.. The result will be undefined";
}

Bool::Bool(bool value)
    : _value(value)
{
    _type = PinProperty::Type::Bool;
}

void Bool::setValue(const double value)
{
    _value = value;
}

void Bool::setValue(const int value)
{
    _value = value;
}

void Bool::setValue(const QString &value)
{
    const QString &str = value.toLower().trimmed();

    if (str == "true")
        _value = true;
    else
        _value = false;
}

String::String(const QString &value)
    : _value(value)
{
    _type = PinProperty::Type::String;
}

void String::setValue(const QString &value)
{
    _value = value;
}

void String::setValue(const int value)
{
    _value = QString::number(value);
}

void String::setValue(const double value)
{
    _value = QString::number(value);
}

void String::setValue(const bool value)
{
    _value = (value) ? "true" : "false";
}

Enum::Enum()
{
    _type = PinProperty::Type::Enum;
}

void Enum::modifyEnumName(const QString &name)
{
    initialize(name);
    forEachLink([&](Link *link) {
        Pin *opposite = link->oppositePin(this);
        if (!isCompatible(opposite))
            delete link;            
    });
}

void Enum::initialize(const QString &name)
{
    _list = NBD_INST.enumValues(name);
    _enumName = name;
    initialized();
}

bool Enum::isCompatible(Pin *to)
{
    PinDecl::Enum *toEnum;

    if (!Pin::isCompatible(to))
        return false;

    toEnum = static_cast<PinDecl::Enum *>(to);
    return (enumName() == toEnum->enumName());
}

Array::Array()
{
    _type = PinProperty::Type::Array;
}

bool Array::isCompatible(Pin *to)
{
    Pin *elem;
    Pin *toElem;
    PinDecl::Array *toArray;
    bool ret = false;

    if (!Pin::isCompatible(to))
        return false;

    toArray = static_cast<PinDecl::Array *>(to);
    elem = NBD_INST.pinTypeToObj(elemType());
    toElem = NBD_INST.pinTypeToObj(toArray->elemType());

    ret = elem->isCompatible(toElem);

    delete elem;
    delete toElem;

    return ret;
}

void Array::onEventConnect(void)
{
    PinDecl::Array *target;

    target = dynamic_cast<PinDecl::Array *>(linkedPins().at(0));
    if (target == nullptr || target->elemType() == PinProperty::Type::Wildcard)
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

Exec::Exec()
{
    _type = PinProperty::Type::Exec;
}

PinDecl::Interface::Interface()
{
    _type = PinProperty::Type::Interface;
}

bool PinDecl::Interface::isCompatible(Pin *to)
{
    // todo : check if interface is compatible (io->io / ! psu->io)
    (void)to;
    return false;
}