#include <GNodeExec.hpp>

GNodeExec::GNodeExec(const QString &name, NodeExecDirection direction)
    : GNodeBasic(name)
{
    setType(NodeProperty::Type::Exec);

    switch (direction) {
        case NodeExecDirection::Bidirectionnal:
            _execIn = addInput<PinDecl::Exec>();
            _execOut = addOutput<PinDecl::Exec>();
            break;
        case NodeExecDirection::Input:
            _execIn = addInput<PinDecl::Exec>();
            break;
        case NodeExecDirection::Output:
            _execOut = addOutput<PinDecl::Exec>();
            break;
        default:
            break;
    }
}

GNodeExec::GNodeExec(const QString &name)
    : GNodeExec(name, NodeExecDirection::Bidirectionnal)
{

}

GNode *GNodeExec::branch(void)
{
    GNode *ret = nullptr;

    forEachOutputPin([&](Pin *pin) {
        if (pin->type() == PinProperty::Type::Exec) {
            pin->forEachLinkedNode([&](GNode *node){ret = node;});
            if (ret)
                return ;
        }
    });
    return ret;
}