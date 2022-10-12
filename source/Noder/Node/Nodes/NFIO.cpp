#include <NFIO.hpp>

IoInit::IoInit()
    : GNodeExec("Initialize IO")
{
    _direction = addInput<PinDecl::Enum>("Direction");
    _value = addInput<PinDecl::Enum>("Value");
    _interface = addInput<PinDecl::Interface>("Object");
    
    _direction->initialize("I/O direction");
    _value->initialize("I/O value");
}

void IoInit::exec(void)
{

}