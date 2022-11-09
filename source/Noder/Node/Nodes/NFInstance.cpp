#include "NFInstance.hpp"

Instance::Instance()
    : GNodeInstance()
{

}

void Instance::exec(void)
{
 /*   switch (_pin->valueType()) {
        case NoderVar::Type::Bool:
        {
            PinDecl::Bool *pin = static_cast<PinDecl::Bool *>(_pin);
            NoderValBool *table = static_cast<NoderValBool *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVar::Type::Int:
        {
            PinDecl::Int *pin = static_cast<PinDecl::Int *>(_pin);
            NoderValInt *table = static_cast<NoderValInt *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVar::Type::Float:
        {
            PinDecl::Float *pin = static_cast<PinDecl::Float *>(_pin);
            NoderValFloat *table = static_cast<NoderValFloat *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVar::Type::String:
        {
            PinDecl::String *pin = static_cast<PinDecl::String *>(_pin);
            NoderValString *table = static_cast<NoderValString *>(_var->defValTable());
            pin->sendValue(table->text());
            break;
        }
        case NoderVar::Type::Enum:
        {
            PinDecl::Enum *pin = static_cast<PinDecl::Enum *>(_pin);
            NoderValEnum *table = static_cast<NoderValEnum *>(_var->defValTable());
            pin->sendValue(table->enumValue());
            break;
        }

        case NoderVar::Type::Wildcard:
        case NoderVar::Type::Interface:
        case NoderVar::Type::Array:
            break ;
    }
    */
}

void Instance::updatePin(void)
{
    PinValue *newPin;
/*
    newPin = addPinFromType(_var->type(), _var->name(), PinProperty::Direction::Output);
    if (_var->type() == NoderVar::Type::Enum) {
        PinDecl::Enum *pin = static_cast<PinDecl::Enum *>(newPin);
        NoderValEnum *value = static_cast<NoderValEnum *>(_var->defValTable());
        //pin->initialize(value->enumName());
        connect(value, &NoderValEnum::enumNameChanged, pin, &PinDecl::Enum::modifyEnumName);
    }

    if (_pin)
        replacePin(_pin, newPin);

    _pin = newPin;

    connect(_var, &NoderSPVariable::nameChanged, _pin, &Pin::setName);
    */
}

void Instance::setVariable(NoderSPVariable *var)
{
    /*
    _var = var;
    connect(var, &NoderSPVariable::typeChanged, this, &Instance::updatePin);
    connect(_var, &NoderSPVariable::dead, this, [&]() {
        deleteLater();
    });
    updatePin();
    */
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
            NoderVar::Type type = static_cast<PinValue *>(pin)->valueType();
            PinProperty::Direction direction = Pin::OppositeDirection(pin->direction());
            addPinFromType(type, name, direction);
        });
    };

    deleteValuePins();

    clonePins(_function->startNode());
    clonePins(_function->endNode());
}