#include "NFFor.hpp"
#include <NoderScene.hpp>

For::For()
    : GNodeBranch("For")
{
    _start = addInput<PinDecl::Int>("Start");
    _end = addInput<PinDecl::Int>("End");
    _scope = addOutput<PinExec>("Scope");
    _done = addOutput<PinExec>("Done");
    _index = addOutput<PinDecl::Int>("Index");
}

GNode *For::branch(void)
{
    // Warn if start > end
    int start;
    int end;
    GNode *ret = nullptr;

    start = _start->value();
    end = _end->value();
    for (int i = start; i < end; i++) {
        _index->sendValue(i);
        
        _scope->forEachLinkedNode([&](GNode *node) {
            scene()->executeSection(node);
            return ;
        });
    }
    _done->forEachLinkedNode([&ret](GNode *node) {
        ret = node;
        return ;
    });
    return ret;
}

ForEachInArray::ForEachInArray()
    : GNodeBranch("For Each Element in Array")
{
    _array = addInput<PinDecl::Array>("Array");
    _scope = addOutput<PinExec>("Scope");
    _done = addOutput<PinExec>("Done");
    _elem = addOutput<PinDecl::Wildcard>("Element");
}

GNode *ForEachInArray::branch(void)
{
    unsigned int end;
    GNode *ret = nullptr;

    end = _array->list().size();
    for (unsigned int i = 0; i < end; i++) {
        switch(_array->elemType()) {
            case NoderVar::Type::Bool:
            {
                PinDecl::Bool *pin = static_cast<PinDecl::Bool *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVar::Type::Int:
            {
                PinDecl::Int *pin = static_cast<PinDecl::Int *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVar::Type::Float:
            {
                PinDecl::Float *pin = static_cast<PinDecl::Float *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVar::Type::String:
            {
                PinDecl::String *pin = static_cast<PinDecl::String *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVar::Type::Enum:
            {
                //PinDecl::Enum *pin = static_cast<PinDecl::Enum *>(_array->list().at(0));
                //_elem->sendValue(pin->value());
                break;
            }
            default:
                break;
        }
        _scope->forEachLinkedNode([&](GNode *node) {
            scene()->executeSection(node);
            return ;
        });
    }
    _done->forEachLinkedNode([&ret](GNode *node) {
        ret = node;
        return ;
    });
    return ret;
}

void ForEachInArray::onEventConnect(void)
{
    NoderVar::Type type;

    if (_elem->valueType() != NoderVar::Type::Wildcard)
        return ;

    type = _array->elemType();
    deletePin(_elem);
    _elem = addPinFromType(type, "Element", PinProperty::Direction::Output);
}

void ForEachInArray::onEventDisconnect(void)
{
    if (_array->linked()) {
        return ;
    }
    deletePin(_elem);
    _elem = addPinFromType(NoderVar::Type::Wildcard, "Element", PinProperty::Direction::Output);
}