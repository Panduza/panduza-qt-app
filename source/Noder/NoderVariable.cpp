#include "NoderVariable.hpp"

NoderVariable::NoderVariable(QWidget *parent)
    : PzaWidget(parent)
{

}

void NoderVariable::createDefValTable(void)
{
    switch (type()) {
        case NoderPanel::Type::Bool:    _defValTable = new NoderValBool(this); break;
        case NoderPanel::Type::Int:     _defValTable = new NoderValInt(this); break;
        case NoderPanel::Type::Float:   _defValTable = new NoderValFloat(this); break;
        case NoderPanel::Type::String:  _defValTable = new NoderValString(this); break;
        case NoderPanel::Type::Enum:    _defValTable = new NoderValEnum(this); break;
        default:                        _defValTable = nullptr;
    }
}

void NoderVariable::setName(const QString &name)
{
    _name = name;
    nameChanged(_name);
}

void NoderVariable::setType(const NoderPanel::Type type)
{
    _type = type;
    typeChanged();
}

NoderValBool::NoderValBool(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _valueBox = addProperty<PzaCheckBox>("Default value");
    updateValue(_valueBox->checkState());
    connect(_valueBox, &PzaCheckBox::stateChanged, this, &NoderValBool::updateValue);
}

void NoderValBool::updateValue(int state)
{
    _value = (state == Qt::Checked) ? true : false;
}

NoderValInt::NoderValInt(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _valueBox = addProperty<PzaSpinBox>("Default value");
    updateValue(_valueBox->value());
    connect(_valueBox, &PzaSpinBox::valueChanged, this, &NoderValInt::updateValue);
}

void NoderValInt::updateValue(int value)
{
    _value = value;
}

NoderValFloat::NoderValFloat(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _valueBox = addProperty<PzaDoubleSpinBox>("Default value");
    updateValue(_valueBox->value());
    connect(_valueBox, &PzaDoubleSpinBox::valueChanged, this, &NoderValFloat::updateValue);
}

void NoderValFloat::updateValue(double value)
{
    _value = value;
}

NoderValString::NoderValString(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _textBox = addProperty<PzaLineEdit>("Default value");
    updateText();
    connect(_textBox, &PzaLineEdit::editingFinished, this, &NoderValString::updateText);
}

void NoderValString::updateText(void)
{
    _text = _textBox->text();
}

NoderValEnum::NoderValEnum(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _enumNameBox = addProperty<PzaComboBox>("Enum");
    _enumValuesBox = addProperty<PzaComboBox>("Default value");
    
    connect(_enumValuesBox, &PzaComboBox::currentIndexChanged, this, &NoderValEnum::updateValuesBox);
    connect(_enumNameBox, &PzaComboBox::currentIndexChanged, this, &NoderValEnum::updateNameBox);
    
    Noder::Get().forEachEnumName([&](const QString &name) {
        _enumNameBox->addItem(name);
    });
    
    _enumNameBox->setCurrentIndex(0);
}

void NoderValEnum::updateNameBox(void)
{
    QString current = _enumNameBox->currentText();
    
    _enumValuesBox->clear();
    Noder::Get().forEachEnumValues(current, [&](const QString &value) {
        _enumValuesBox->addItem(value);
    });
    _enumValuesBox->setCurrentIndex(0);
    _enumName = current;
    enumNameChanged(_enumName);
}

void NoderValEnum::updateValuesBox(void)
{
    _enumValue = _enumValuesBox->currentText();
}