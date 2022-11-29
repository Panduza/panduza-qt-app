#include <PinExec.hpp>

using namespace PinDecl;

Exec::Exec()
{
    _pinType = PinProperty::Type::Exec;
}

bool Exec::isCompatible(Pin *to)
{
    return Pin::isCompatible(to);
}