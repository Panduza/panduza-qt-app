#include "NoderFunction.hpp"

NoderFunction::NoderFunction(QWidget *parent)
    : NoderView(parent)
{
    _panelName = new PzaLineEdit("New function", this);
    connect(_panelName, &PzaLineEdit::editingFinished, this, &NoderFunction::setName);
    _name = _panelName->text();
}