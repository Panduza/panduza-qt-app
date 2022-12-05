#include "NFInstance.hpp"

VariableInstance::VariableInstance()
    : GNodeInstance()
{

}

void VariableInstance::exec(void)
{
 /*   switch (_pin->valueType()) {
        case NoderVarProps::Type::Bool:
        {
            PinDecl::Ref::Bool *pin = static_cast<PinDecl::Ref::Bool *>(_pin);
            NoderValBool *table = static_cast<NoderValBool *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVarProps::Type::Int:
        {
            PinDecl::Ref::Int *pin = static_cast<PinDecl::Ref::Int *>(_pin);
            NoderValInt *table = static_cast<NoderValInt *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVarProps::Type::Float:
        {
            PinDecl::Ref::Float *pin = static_cast<PinDecl::Ref::Float *>(_pin);
            NoderValFloat *table = static_cast<NoderValFloat *>(_var->defValTable());
            pin->sendValue(table->value());
            break;
        }
        case NoderVarProps::Type::String:
        {
            PinDecl::Ref::String *pin = static_cast<PinDecl::Ref::String *>(_pin);
            NoderValString *table = static_cast<NoderValString *>(_var->defValTable());
            pin->sendValue(table->text());
            break;
        }
        case NoderVarProps::Type::Enum:
        {
            PinDecl::Ref::Enum *pin = static_cast<PinDecl::Ref::Enum *>(_pin);
            NoderValEnum *table = static_cast<NoderValEnum *>(_var->defValTable());
            pin->sendValue(table->enumValue());
            break;
        }

        case NoderVarProps::Type::Wildcard:
        case NoderVarProps::Type::Interface:
        case NoderVarProps::Type::Array:
            break ;
    }
    */
}

void VariableInstance::newVariable(NoderVariable *var)
{
    PinVariable *newPin;

    newPin = addVariable(var->name(), PinProperty::Direction::Output, var->props());
    if (_pin)
        replacePin(_pin, newPin);
    _pin = newPin;

    _var = var;

    connect(_var, &NoderVariable::nameChanged, _pin, &Pin::setName);
}

void VariableInstance::setVariableEntry(NoderSPVariableEntry *entry)
{
    _varEntry = entry;
    connect(_varEntry, &NoderSPVariableEntry::varChanged, this, &VariableInstance::newVariable);
    connect(_varEntry, &NoderSPVariableEntry::destroyed, this, &QObject::deleteLater);
    newVariable(entry->elem());
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
        kill();
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
            const NoderVarProps &varProps = static_cast<PinVariable *>(pin)->varProps();
            PinProperty::Direction direction = Pin::OppositeDirection(pin->direction());
            addVariable(name, direction, varProps);
        });
    };

    deleteValuePins();

    clonePins(_function->startNode());
    clonePins(_function->endNode());
}