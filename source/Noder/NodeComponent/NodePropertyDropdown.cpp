#include <NodePropertyDropdown.hpp>
#include <Node.hpp>

NodePropertyDropdown::NodePropertyDropdown(QString name, Node *parent)
    : NodeProperty(name, parent)
{
    _combo = addWidget<PzaComboBox>(1, 0);
    _combo->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    _layout->setColumnStretch(2, 1);
    connect(_combo, &PzaComboBox::clicked, this, &NodePropertyDropdown::activate);
}

NodePropertyDropdown::NodePropertyDropdown(Node *parent)
    : NodePropertyDropdown(nullptr, parent)
{

}

void NodePropertyDropdown::activate(void)
{
    _node->forEachProperties([](NodeProperty *property)
    {
        property->proxy()->setZValue(0);
    });
    proxy()->setZValue(1);
}

NodePropertyDropdown::~NodePropertyDropdown()
{

}