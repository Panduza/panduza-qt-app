#include "NoderSPVariable.hpp"

NoderSPVariable::NoderSPVariable(QWidget *parent)
    : PzaWidget(parent)
{

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
    _enumValuesBox = addProperty<PzaComboBox>("Default value");
    
    connect(_enumValuesBox, &PzaComboBox::currentIndexChanged, this, &NoderValEnum::updateValuesBox);
}

void NoderValEnum::updateNameBox(void)
{
   
}

void NoderValEnum::updateValuesBox(void)
{
    _enumValue = _enumValuesBox->currentText();
}

NoderValArray::NoderValArray(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    //PzaPushButton *

    layout->addWidget(new PzaLabel("hello"));
    layout->addWidget(new PzaComboBox());
}