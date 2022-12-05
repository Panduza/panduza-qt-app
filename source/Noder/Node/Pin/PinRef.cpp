#include <PinRef.hpp>
#include <Link.hpp>

using namespace PinDecl::Ref;

PinRef::PinRef()
    : PinVariable()
{
    _varProps.container = NoderVarProps::Container::Reference;
}

bool PinRef::isCompatible(Pin *to)
{
    if (PinVariable::isCompatible(to) == false)
        return false;

    return Noder::IsTypeCompatible(_varProps.type, static_cast<PinRef *>(to)->varProps().type);
}

Float::Float()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::Float;
}

void Float::setupWidgets(void)
{
    Pin::setupWidgets();

    _box = new PzaDoubleSpinBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void Float::setValue(const int value)
{
    setValue((double)value);
}

void Float::setValue(const bool value)
{
    setValue((double)value);
}

void Float::setValue(const QString &value)
{
    bool ok;
    double res;

    res = value.toDouble(&ok);
    if (ok)
        setValue(res);
    else
        qWarning() << "Input string" << value << "cannot be converted to float.. The result will be undefined";
}

Int::Int()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::Int;
}

void Int::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaSpinBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void Int::setValue(const double value)
{
    setValue((int)value);
}

void Int::setValue(const bool value)
{
    setValue((bool)value);
}

void Int::setValue(const QString &value)
{
    bool ok;
    int res;

    res = value.toInt(&ok);
    if (ok)
        setValue(res);
    else
        qWarning() << "Input string" << value << "cannot be converted to an integer.. The result will be undefined";
}

Bool::Bool()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::Bool;
}

void Bool::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaCheckBox(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _grid->setColumnStretch(1, 1);
    _grid->addWidget(_box, 0, 1);
}

void Bool::setValue(const double value)
{
    setValue((bool)value);
}

void Bool::setValue(const int value)
{
    setValue((bool)value);
}

void Bool::setValue(const QString &value)
{
    const QString &str = value.toLower().trimmed();

    if (str == "true")
        setValue(true);
    else
        setValue(false);
}

String::String()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::String;
}

void String::setupWidgets(void)
{
    Pin::setupWidgets();
    
    _box = new PzaLineEdit(this);
    _box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);
}

void String::setValue(const int value)
{
    setValue(QString::number(value));
}

void String::setValue(const double value)
{
    setValue(QString::number(value));
}

void String::setValue(const bool value)
{
    (value) ? setValue("true") : setValue("false");
}

Enum::Enum()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::Enum;
}

void Enum::setupWidgets(void)
{
    Pin::setupWidgets();

    _box = new PzaComboBox(this);
    _box->setFixedWidth(100);
    _grid->addWidget(_box, 0, 1);

    connect(this, &PinVariable::subTypeChanged, this, [&]() {
        Noder::ForEachEnumValues(_varProps.subType, [&](const QString &value) {
            _box->insertItem(0, value);
        });
        forEachLink([&](Link *link) {
            Pin *opposite = link->oppositePin(this);
            if (!isCompatible(opposite))
                link->kill();            
        });
    });
}

void Enum::modifyEnumName(const QString &name)
{
    initialize(name);
}

void Enum::initialize(const QString &name)
{
    _list = Noder::Get().EnumValues(name);
    _enumName = name;
    initialized();
}

bool Enum::isCompatible(Pin *to)
{
    if (PinRef::isCompatible(to) == false)
        return false;

    return (_varProps.subType == static_cast<PinRef *>(to)->varProps().subType);
}

Interface::Interface()
    : PinRef()
{
    _varProps.type = NoderVarProps::Type::Interface;
}

bool Interface::isCompatible(Pin *to)
{
    if (PinRef::isCompatible(to) == false)
        return false;

    return (_varProps.subType == static_cast<PinRef *>(to)->varProps().subType);
}