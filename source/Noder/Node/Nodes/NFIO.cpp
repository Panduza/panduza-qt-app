#include "NFIO.hpp"

IoInit::IoInit()
    : GNodeExec("Initialize IO")
{
    _direction = addVariableInput<PinDecl::Ref::Enum>("Direction", "I/O direction");
    _value = addVariableInput<PinDecl::Ref::Enum>("Value", "I/O value");
    _interface = addInput<PinDecl::Ref::Interface>("Object");
    
    _direction->initialize("I/O direction");
    _value->initialize("I/O value");
}

void IoInit::exec(void)
{

}