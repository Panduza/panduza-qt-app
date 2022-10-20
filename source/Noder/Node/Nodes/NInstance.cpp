#include "NInstance.hpp"

Instance::Instance(NoderVariable *var)
    : GNodeInstance("")
{
    PinProperty::Type type;

    type = NBD_INST.panelTypeToPinType(var->type());
    _pin = addPinFromType(type, var->name(), PinProperty::Direction::Output);
    setVariable(var);
}

void Instance::exec(void)
{

}

void Instance::setVariable(NoderVariable *var)
{
    connect(var, &NoderVariable::nameChanged, this, [&]() {
        _pin->setName(var->name());
    });

    _ref = var;
}