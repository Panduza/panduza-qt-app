#include "NFString.hpp"

Print::Print()
    : GNodeExec("Print String")
{
    addMultiInput<PinDecl::Ref::String>(_list, "String", 1);
}

void Print::exec(void)
{
    PinDecl::Ref::String *string;
    QString res;

    for (auto const &elem : _list) {
        string = static_cast<PinDecl::Ref::String *>(elem);
        res = res + string->value();
    }
    qDebug() << res;
}

Cat::Cat()
    : GNodeOp("Concatenate String")
{
    _result = addOutput<PinDecl::Ref::String>("Result");
    _spacer = addInput<PinDecl::Ref::String>("Spacer");
    addMultiInput<PinDecl::Ref::String>(_list, "String");
}

void Cat::exec(void)
{
    QString res;
    unsigned int len;
    PinDecl::Ref::String *elem;
    
    len = _list.size();
    for (unsigned int i = 0; i < len; i++) {
        elem = static_cast<PinDecl::Ref::String *>(_list[i]);
        res = res + elem->value();
        if (i + 1 != len)
            res = res + _spacer->value();
    }
    _result->sendValue(res);
}