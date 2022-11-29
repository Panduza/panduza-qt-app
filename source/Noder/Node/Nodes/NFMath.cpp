#include "NFMath.hpp"

ABasicOperation::ABasicOperation(const QString &name)
    : GNodeOp(name)
{
    addMultiInput<PinDecl::Ref::Float>(_list, "", 2);
    _result = addOutput<PinDecl::Ref::Float>("Result");
}

void ABasicOperation::exec(void)
{
    double val = 0.0f;
    PinDecl::Ref::Float *pin;
    unsigned int len;

    len = _list.size();

    pin = static_cast<PinDecl::Ref::Float *>(_list[0]);
    val = pin->value();

    for (unsigned int i = 1; i < len; i++) {
        pin = static_cast<PinDecl::Ref::Float *>(_list[i]);
        val = mathOp(val, pin->value());
    }

    _result->sendValue(val);
}