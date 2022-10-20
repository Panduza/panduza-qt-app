#include "PzaProperty.hpp"

PzaProperty::PzaProperty(const QString &name, QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);

    if (name.length() > 0) {
        _name = new PzaLabel(name, this);
        _layout->addWidget(_name);
    }
}

PzaPropertyComboBox::PzaPropertyComboBox(const QString &name, QWidget *parent)
    : PzaProperty(name, parent)
{
    _prop = new PzaComboBox(this);

    _layout->addWidget(_prop);
}