#include "NFIf.hpp"

If::If()
    : GNodeBranch("If")
{
    _true = addOutput<PinExec>("True");
    _false = addOutput<PinExec>("False");
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