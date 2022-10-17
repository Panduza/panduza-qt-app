#include "NoderVariable.hpp"

NoderVariable::NoderVariable(NoderPanel::Type type, QWidget *parent)
    : QWidget(parent),
    _type(type)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);

    PzaLabel *typeName = new PzaLabel(NBD_INST.varTypeName(type), this);
    typeName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    _varLabel = new PzaLineEdit("New variable", this);
    _varLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_varLabel, &PzaLineEdit::focusIn, this, [&]() {
        activate();
    });
    connect(_varLabel, &PzaLineEdit::editingFinished, this, [&]() {
        _name = _varLabel->text();
        nameChanged();
    });

    _varLabel->editingFinished();
    
    _colorFrame = new QFrame(this);
    _colorFrame->setFixedSize(8, 8);
    _colorFrame->setStyleSheet(
        "background-color: " + NBD_INST.varColor(type).name() + ";"
        "border-radius: 3px;"
    );
    _colorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _layout->setContentsMargins(2, 2, 2, 2);
    _layout->setSpacing(10);

    _layout->addWidget(_varLabel);
    _layout->addWidget(_colorFrame);
    _layout->addWidget(typeName);
}

NoderVariable::NoderVariable(QWidget *parent)
    : NoderVariable(NoderPanel::Type::Bool, parent)
{

}

void NoderVariable::setSelected(bool state)
{
    if (state == true) {
        setStyleSheet("background-color: #1B2426");
    }
    else
        setStyleSheet("background-color: transparent");
}

void NoderVariable::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        activate();
}