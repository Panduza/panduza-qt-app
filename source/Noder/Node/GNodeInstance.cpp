#include <GNodeInstance.hpp>

GNodeInstance::GNodeInstance()
    : GNode("Instance")
{
    _type = NodeProperty::Type::Instance;
    _hasTitle = false;

    setup();
}