#include "NFIf.hpp"

If::If()
    : GNodeBranch("If")
{
    _true = addOutput<PinDecl::Exec>("True");
    _false = addOutput<PinDecl::Exec>("False");
    _cond = addInput<PinDecl::Bool>("Condition");
}

GNode *If::branch(void)
{
    bool val;

    val = _cond->value();
    if (val)
        return (_true->linkedNodes().size() > 0) ? _true->linkedNodes().front() : nullptr;
    else
        return (_false->linkedNodes().size() > 0) ? _false->linkedNodes().front() : nullptr;
}