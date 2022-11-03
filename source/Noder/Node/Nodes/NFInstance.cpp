#include "NFInstance.hpp"

Instance::Instance()
    : GNodeInstance()
{

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

    type = Noder::Get().panelTypeToPinType(_var->type());
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
    connect(_var, &NoderVariable::dead, this, [&]() {
        deleteLater();
    });
    updatePin();
}

FuncInstance::FuncInstance()
    : GNodeExec("")
{

}

void FuncInstance::setFunction(NoderFunction *func)
{
    _function = func;
    connect(_function, &NoderFunction::updated, this, &FuncInstance::updateNode);
    connect(_function, &NoderFunction::nameChanged, this, &FuncInstance::refreshUserName);
    connect(_function, &NoderFunction::dead, this, [&]() {
        deleteLater();
    });
    refreshUserName(_function->name());
    updateNode();
}

void FuncInstance::updateNode(void)
{
    auto clonePins = [&](GNode *node) {
        node->forEachPin([&](Pin *pin) {
            if (pin->type() == PinProperty::Type::Exec)
                return;
            const QString &name = pin->name();
            PinProperty::Type type = pin->type();
            PinProperty::Direction direction = Pin::OppositeDirection(pin->direction());
            addPinFromType(type, name, direction);
        });
    };

    deleteValuePins();

    clonePins(_function->startNode());
    clonePins(_function->endNode());
}