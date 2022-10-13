#include "NFMath.hpp"

ABasicOperation::ABasicOperation(const QString &name)
    : GNodeOp(name)
{
    addMultiInput<PinDecl::Float>(&_list);
    _result = addOutput<PinDecl::Float>("Result");
}

void ABasicOperation::exec(void)
{
    double val = 0.0f;
    PinDecl::Float *pin;
    unsigned int len;

    len = _list.size();

    pin = static_cast<PinDecl::Float *>(_list[0]);
    val = pin->value();

    for (unsigned int i = 1; i < len; i++) {
        pin = static_cast<PinDecl::Float *>(_list[i]);
        val = mathOp(val, pin->value());
    }

    _result->sendValue(val);
}