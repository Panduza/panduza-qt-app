#include <NodeEntry.hpp>
#include <Link.hpp>
#include <Node.hpp>

NodeEntry::NodeEntry(QString name, enum NodeEntryDirection direction, Node *parent)
    : NodeComponent(name, parent),
    _direction(direction),
    _link(nullptr),
    _type(NoType)
{

}

NodeEntry::NodeEntry(enum NodeEntryDirection direction, Node *parent)
    : NodeEntry(nullptr, direction, parent)
{

}

void NodeEntry::hideWidgets(void)
{
    QWidget *w;

    for (int i = 0; i < _layout->count(); i++) {
        w = _layout->itemAt(i)->widget();
        if (w != _name) {
            w->hide();
        }
    }
    adjustSize();
}

void NodeEntry::showWidgets()
{
    QWidget *w;

    for (int i = 0; i < _layout->count(); i++) {
        w = _layout->itemAt(i)->widget();
        if (w != _name) {
            w->show();
        }
    }
    adjustSize();
}

void NodeEntry::createLink(void)
{
    Link *link;

    link = new Link(_node->scene(), _node->mapToScene(plugCenter()), this);

    link->grabMouse();
    link->grabKeyboard();
}

void NodeEntry::connectLink(Link *link)
{
    _link = link;
    hideWidgets();
    _node->refreshNode();
    _node->update();
}

void NodeEntry::unlink(void)
{
    _link = nullptr;
    showWidgets();
    _node->refreshNode();
    _node->update();
}

void NodeEntry::disconnectLink(void)
{
    _link->disconnect(this);
    _link->grabMouse();
    _link->grabKeyboard();
    unlink();
}

void NodeEntry::moveLink(void)
{
    _link->move();
}

Node *NodeEntry::linkedNode(void)
{
    return (linked()) ? linkedEntry()->node() : nullptr;
}

NodeEntry *NodeEntry::linkedEntry(void)
{
    return (_direction == Input) ? _link->inEntry() : _link->outEntry();
}

NodeEntry::~NodeEntry()
{
    if (linked())
        _link->entryDied(this);
}