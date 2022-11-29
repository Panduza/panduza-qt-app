#include <PinArray.hpp>

using namespace PinDecl::Array;

PinArray::PinArray()
    : PinVariable()
{
    _varProps.container = NoderVarProps::Container::Array;
}

bool PinArray::isCompatible(Pin *to)
{
    return false;
}

Bool::Bool()
{
    _varProps.type = NoderVarProps::Type::Bool;
}

Int::Int()
{
    _varProps.type = NoderVarProps::Type::Int;
}

Float::Float()
{
    _varProps.type = NoderVarProps::Type::Float;
}

String::String()
{
    _varProps.type = NoderVarProps::Type::String;
}

Enum::Enum()
{
    _varProps.type = NoderVarProps::Type::Enum;
}

Interface::Interface()
{
    _varProps.type = NoderVarProps::Type::Interface;
}

Wildcard::Wildcard()
{
    _varProps.type = NoderVarProps::Type::Wildcard;
}