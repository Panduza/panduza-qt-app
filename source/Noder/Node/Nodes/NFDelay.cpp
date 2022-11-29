
#include "NFDelay.hpp"

Delay::Delay()
    : GNodeExec("Delay")
{
    _delay = addInput<PinDecl::Ref::Float>("Delay");
}

void Delay::exec(void)
{
    QThread::sleep(_delay->value());
}