#include "NFArray.hpp"

Array::Array()
    : GNodeOp("Array"),
    _type(NoderVar::Type::Wildcard)
{
    addMultiInput<PinDecl::Wildcard>("", &_list, 2);
    _result = addOutput<PinDecl::Array>("Result");
}

void Array::exec(void)
{
    _result->sendValue(_list);
}

void Array::onEventConnect()
{
    int index;
    int listSize;
    PinValue *from = nullptr;
    PinValue *to = nullptr;
    struct multiPin *s;

    if (_type != NoderVar::Type::Wildcard)
        return ;

    forEachInputPin([&](Pin *pin) {
        PinValue *valuePin = static_cast<PinValue *>(pin);
        if (pin->linked()) {
            _type = valuePin->valueType();
            from = static_cast<PinValue *>(pin->linkedPins().front());
            to = static_cast<PinValue *>(pin);
            return ;
        }
    });

    if (!to || !from)
        return ;

    listSize = _list.size();
    index = PzaUtils::IndexInVector<PinValue *>(_list, to);
    for (auto const &pin: _list) {
        deletePin(pin);
    }
    _list.clear();

    _type = from->valueType();

    for (int i = 0; i < listSize; i++) {
        PinValue *pin;

        pin = addPinFromType(from->valueType(), " " + QString::number(i), PinProperty::Direction::Input);
        if (i == index)
            Pin::CreateLink(from, pin);
        _list.push_back(pin);
    }

    _result->setElemType(_type);

    s = findMultiPinFromList(&_list);
    if (s)
        s->type = _type;

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
        }
    });
    if (!empty)
        return ;

    _type = NoderVar::Type::Wildcard;

    listSize = _list.size();
    for (auto const &pin: _list) {
        deletePin(pin);
    }
    _list.clear();

    for (unsigned int i = 0; i < listSize; i++) {
        PinValue *pin = addPinFromType(_type, " " + QString::number(i), PinProperty::Direction::Input);
        _list.push_back(pin);
    }

    _result->setElemType(_type);

    s = findMultiPinFromList(&_list);
    if (s)
        s->type = _type;

    _result->removeLinks();
}

Array::~Array()
{

}