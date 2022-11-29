
#include <Pin.hpp>
#include <GNode.hpp>
#include <Link.hpp>
#include <Store.hpp>

#include "MainWindow.hpp"

Pin::Pin()
    : PzaWidget()
{
    setObjectName("pin");
    Store::Get().style.bindStyleSheet(this);
}

void Pin::setName(const QString &name)
{
    _name = name;
    _label->setText(name);
    adjustSize();
    _node->refreshNode();
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
    forEachLink([&](Link *link) {
        link->kill();
    });
}

void Pin::disconnectLink(const QPointF &pos)
{
    Link *link;
    Pin *from;

    link = _links.front();

    from = link->oppositePin(this);
    link->kill();

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

bool Pin::isCompatible(Pin *to)
{
    return (to->type() == _pinType);
}

void Pin::kill(void)
{
    _dead = true;
    _proxy->deleteLater();
    removeLinks();
    deleteLater();
}

PinVariable::PinVariable()
    : Pin()
{
    _pinType = PinProperty::Type::Variable;
}

bool PinVariable::isCompatible(Pin *to)
{
    if (Pin::isCompatible(to) == false)
        return false;

    return (static_cast<PinVariable *>(to)->varProps().container == _varProps.container);
}