#include "NFArray.hpp"

Array::Array()
    : GNodeOp("Array")
{
    _var.type = NoderVarProps::Type::Wildcard;

    addMultiInput<PinDecl::Ref::Wildcard>(_list, "", 2);
    _result = addVariableOutput<PinDecl::Array::Wildcard>("Result");
}

void Array::exec(void)
{
    //_result->sendValue(_list);
}

void Array::onEventConnect()
{
    int index;
    int listSize;
    PinRef *from = nullptr;
    PinRef *to = nullptr;
    PinRef *pin;
    struct multiPin *s;

    if (_var.type != NoderVarProps::Type::Wildcard)
        return ;

    s = findMultiPinFromList(&_list);
    if (!s)
        return ;

    forEachInputPin([&](Pin *item) {
        PinRef *pin = static_cast<PinRef *>(item);
        if (pin->linked()) {
            from = static_cast<PinRef *>(pin->linkedPins().front());
            _var = from->varProps();
            to = static_cast<PinRef *>(pin);
            return ;
        }
    });

    if (!to || !from)
        return ;

    s->var = _var;

    listSize = _list.size();
    index = PzaUtils::IndexInVector<PinRef *>(_list, to);
    PzaUtils::ForEachDeleteInVector<PinRef *>(_list, [&](PinRef *pin) {
        deletePin(pin);
    });

    for (int i = 0; i < listSize; i++) {
        pin = addRefInput(QString::number(i), from->varProps().type, from->varProps().subType);
        if (i == index)
            Pin::CreateLink(from, pin);
        _list.push_back(pin);
    }

    PinArray *newRes = addArrayOutput("Result", _var.type, _var.subType);
    replacePin(_result, newRes);
    _result = newRes;

    _result->forEachLinkedPin([](Pin *pin) {
        pin->onEventConnect();
    });
}

void Array::onEventDisconnect()
{
    unsigned int listSize;
    bool empty = true;
    struct multiPin *s;

    forEachInputPin([&](Pin *pin) {
        if (pin->linked()) {
            empty = false;
            return ;
        }
    });
    if (!empty)
        return ;

    s = findMultiPinFromList(&_list);
    if (!s)
        return ;

    listSize = _list.size();
    for (auto const &pin: _list) {
        deletePin(pin);
    }
    _list.clear();

    for (unsigned int i = 0; i < listSize; i++) {
        PinRef *pin = addInput<PinDecl::Ref::Wildcard>(" " + QString::number(i));
        _list.push_back(pin);
    }

    _var.type = NoderVarProps::Type::Wildcard;

    PinDecl::Array::Wildcard *newRes = addOutput<PinDecl::Array::Wildcard>("Result");
    replacePin(_result, newRes);
    _result = newRes;
}

Array::~Array()
{

}