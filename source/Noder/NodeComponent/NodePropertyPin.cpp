#include <NodePropertyPin.hpp>

NodePropertyPin::NodePropertyPin(QString name, Node *parent)
    : NodeProperty(nullptr, parent)
{
    _more_btn = addWidget<QPushButton>(0, 0);
    _more_btn->setObjectName("plus");

    _less_btn = addWidget<QPushButton>(0, 1);
    _less_btn->setObjectName("minus");

    QLabel *label = addWidget<QLabel>(0, 2);
    label->setText("Pin");

    _layout->setColumnStretch(2, 0);
    _layout->setSpacing(5);

    connect(_more_btn, &QPushButton::clicked, this, [&]{more();});
    connect(_less_btn, &QPushButton::clicked, this, [&]{less();});
}

NodePropertyPin::NodePropertyPin(Node *parent)
    : NodePropertyPin(nullptr, parent)
{

}

NodePropertyPin::~NodePropertyPin()
{

}