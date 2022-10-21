#include "NoderVariable.hpp"

NoderVariable::NoderVariable(NoderPanel::Type type, QWidget *parent)
    : PzaWidget(parent)
{
    int index = 0;

    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);
    _propTable = new PzaPropertyTable(this);
    _propName = _propTable->addProperty<PzaLabel>("Name");
    _propType = _propTable->addProperty<PzaComboBox>("Type");
    _typeName = new PzaLabel(NBD_INST.varTypeName(type), this);
    _varLabel = new PzaLineEdit("New variable", this);
    _colorFrame = new QFrame(this);
    
    setType(type);
    
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
    _colorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _layout->setContentsMargins(2, 2, 2, 2);
    _layout->setSpacing(10);

    _layout->addWidget(_varLabel);
    _layout->addWidget(_colorFrame);
    _layout->addWidget(_typeName);

    NBD_INST.forEachVarType([&](NoderPanel::Type type) {
        _propType->insertItem(0, NBD_INST.varTypeName(type));
    });
    index = _propType->findText(NBD_INST.varTypeName(_type));

    _propType->setCurrentIndex(index);

    createDefValTable();

    connect(_propType, &PzaComboBox::currentIndexChanged, this, [&](int index){
        const QString &typeName = _propType->itemText(index);
        setType(NBD_INST.varTypeFromName(typeName));
    });
}

void NoderVariable::setName(const QString &name)
{
    _name = name;
    _propName->setText(name);
    nameChanged(_name);
}

void NoderVariable::setType(const NoderPanel::Type type)
{
    _type = type;
    _colorFrame->setStyleSheet(
        "background-color: " + NBD_INST.varColor(type).name() + ";"
        "border-radius: 3px;"
    );
    _typeName->setText(NBD_INST.varTypeName(type));
    typeChanged();
}

void NoderVariable::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;

    mimeData->setData("noder/variable", "");
    mimeData->setDataPtr(this);
    drag->setMimeData(mimeData);
    drag->exec();
}

void NoderVariable::createDefValTable(void)
{
    switch (_type) {
        case NoderPanel::Type::Bool:
            _defValTable = new NoderValBool(this); break;
        case NoderPanel::Type::Int:
            _defValTable = new NoderValInt(this); break;
        case NoderPanel::Type::Float:
            _defValTable = new NoderValFloat(this); break;
        case NoderPanel::Type::String:
            _defValTable = new NoderValString(this); break;
        case NoderPanel::Type::Enum:
            _defValTable = new NoderValEnum(this); break;
        default:
            _defValTable = nullptr; break;
    }
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

NoderValBool::NoderValBool(QWidget *parent)
    : PzaPropertyTable(parent)
{
    _valueBox = addProperty<PzaCheckBox>("Default value");
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
    
    NBD_INST.forEachEnumName([&](const QString &name) {
        _enumNameBox->addItem(name);
    });
    
    _enumNameBox->setCurrentIndex(0);
}

void NoderValEnum::updateNameBox(void)
{
    QString current = _enumNameBox->currentText();
    
    _enumValuesBox->clear();
    NBD_INST.forEachEnumValues(current, [&](const QString &value) {
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