#include "NFString.hpp"

Print::Print()
    : GNodeExec("Print String")
{
    addMultiInput<PinDecl::String>("String", &_list, 1);
}

void Print::exec(void)
{
    PinDecl::String *string;
    QString res;

    for (auto const &elem : _list) {
        string = static_cast<PinDecl::String *>(elem);
        res = res + string->value();
    }
    qDebug() << res;
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