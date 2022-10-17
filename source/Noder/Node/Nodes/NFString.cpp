#include "NFString.hpp"

Print::Print()
    : GNodeExec("Print String")
{
    _string = addInput<PinDecl::String>("String");
}

void Print::exec(void)
{
    qDebug() << _string->value();
}

Cat::Cat()
    : GNodeOp("Concatenate String")
{
    _result = addOutput<PinDecl::String>("Result");
    _spacer = addInput<PinDecl::String>("Spacer");
    addMultiInput<PinDecl::String>("String", &_list);
}

void Cat::exec(void)
{
    QString res;
    unsigned int len;
    PinDecl::String *elem;
    
    len = _list.size();
    for (unsigned int i = 0; i < len; i++) {
        elem = static_cast<PinDecl::String *>(_list[i]);
        res = res + elem->value();
        if (i + 1 != len)
            res = res + _spacer->value();
    }
    _result->sendValue(res);
}