#include "NInstance.hpp"

Instance::Instance(NoderVariable *var)
    : GNodeInstance("")
{
      setVariable(var);
      updatePin();
}

void Instance::exec(void)
{
    switch (_pin->type()) {
        case PinProperty::Type::Bool:
        {
            PinDecl::Bool *pin = static_cast<PinDecl::Bool *>(_pin);
            NoderValBool *table = static_cast<NoderValBool *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case PinProperty::Type::Int:
        {
            PinDecl::Int *pin = static_cast<PinDecl::Int *>(_pin);
            NoderValInt *table = static_cast<NoderValInt *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case PinProperty::Type::Float:
        {
            PinDecl::Float *pin = static_cast<PinDecl::Float *>(_pin);
            NoderValFloat *table = static_cast<NoderValFloat *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case PinProperty::Type::String:
        {
            PinDecl::String *pin = static_cast<PinDecl::String *>(_pin);
            NoderValString *table = static_cast<NoderValString *>(_var->defValTable());
            pin->sendValue(table->text());
            break;
        }
        case PinProperty::Type::Enum:
        {
            PinDecl::Enum *pin = static_cast<PinDecl::Enum *>(_pin);
            NoderValEnum *table = static_cast<NoderValEnum *>(_var->defValTable());
            pin->sendValue(table->enumValue());
            break;
        }
        default:
            break;
    }
}

void Instance::updatePin(void)
{
    PinProperty::Type type;
    Pin *newPin;

    type = NBD_INST.panelTypeToPinType(_var->type());
    newPin = addPinFromType(type, _var->name(), PinProperty::Direction::Output);
    if (type == PinProperty::Type::Enum) {
        PinDecl::Enum *pin = static_cast<PinDecl::Enum *>(newPin);
        NoderValEnum *value = static_cast<NoderValEnum *>(_var->defValTable());
        pin->initialize(value->enumName());
        connect(value, &NoderValEnum::enumNameChanged, pin, &PinDecl::Enum::modifyEnumName);
    }

    if (_pin) {
        replacePin(_pin, newPin);
    }

    _pin = newPin;

    connect(_var, &NoderVariable::nameChanged, _pin, &Pin::setName);
}

void Instance::setVariable(NoderVariable *var)
{
    _var = var;
    connect(var, &NoderVariable::typeChanged, this, &Instance::updatePin);
}