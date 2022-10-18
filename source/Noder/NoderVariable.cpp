#include "NoderVariable.hpp"

NoderVariable::NoderVariable(NoderPanel::Type type, QWidget *parent)
    : QWidget(parent),
    _type(type)
{
    PzaLabel *typeName;

    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);
    _propTable = new PzaPropertyTable(this);
    _propName = _propTable->addProperty<PzaLabel>("Name");
    _propType = _propTable->addProperty<PzaComboBox>("Type");
    _typeName = new PzaLabel(NBD_INST.varTypeName(type), this);
    _varLabel = new PzaLineEdit("New variable", this);
    _colorFrame = new QFrame(this);
    
    _typeName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    _varLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_varLabel, &PzaLineEdit::focusIn, this, [&]() {
        activate();
    });
    connect(_varLabel, &PzaLineEdit::editingFinished, this, [&]() {
        setName(_varLabel->text());
    });

    _varLabel->editingFinished();
    
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
    _layout->addWidget(_typeName);

    
    connect(_propType, &PzaComboBox::currentIndexChanged, this, [&](int index){
        const QString &typeName = _propType->itemText(index);
        setType(NBD_INST.varTypeFromName(typeName));
    });

    NBD_INST.forEachVarType([&](NoderPanel::Type type) {
        _propType->insertItem(0, NBD_INST.varTypeName(type));
    });
    _propType->setCurrentIndex(0);
}

NoderVariable::NoderVariable(QWidget *parent)
    : NoderVariable(NoderPanel::Type::Bool, parent)
{

}

void NoderVariable::setName(const QString &name)
{
    _name = name;
    _propName->setText(name);
}

void NoderVariable::setType(const NoderPanel::Type type)
{
    _type = type;
    _colorFrame->setStyleSheet(
        "background-color: " + NBD_INST.varColor(type).name() + ";"
        "border-radius: 3px;"
    );
    _typeName->setText(NBD_INST.varTypeName(type));
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