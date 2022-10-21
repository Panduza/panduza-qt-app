#include <GNodeInstance.hpp>

GNodeInstance::GNodeInstance(const QString &name)
    : GNode(name)
{
    _type = NodeProperty::Type::Instance;
    _hasTitle = false;

    setup();
}