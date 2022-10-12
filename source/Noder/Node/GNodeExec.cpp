#include <GNodeExec.hpp>

GNodeExec::GNodeExec(const QString &name, bool in, bool out)
    : GNode(name)
{
    _type = NodeProperty::Type::Exec;

    if (in)
        _execIn = addInput<PinDecl::Exec>("");
    if (out)
        _execOut = addOutput<PinDecl::Exec>("");

    setup();
}

GNodeExec::GNodeExec(const QString &name)
    : GNodeExec(name, 1, 1)
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