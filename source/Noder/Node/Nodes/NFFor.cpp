#include "NFFor.hpp"
#include <NoderScene.hpp>

For::For()
    : GNodeBranch("For")
{
    _start = addInput<PinDecl::Ref::Int>("Start");
    _end = addInput<PinDecl::Ref::Int>("End");
    _done = addOutput<PinDecl::Exec>("Done");
    _scope = addOutput<PinDecl::Exec>("Scope");
    _index = addOutput<PinDecl::Ref::Int>("Index");
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
    //_array = addInput<PinDecl::Ref::Array>("Array");
    _scope = addOutput<PinDecl::Exec>("Scope");
    _done = addOutput<PinDecl::Exec>("Done");
    _elem = addOutput<PinDecl::Ref::Wildcard>("Element");
}

GNode *ForEachInArray::branch(void)
{
    unsigned int end;
    GNode *ret = nullptr;
    /*

    end = _array->list().size();
    for (unsigned int i = 0; i < end; i++) {
        switch(_array->elemType()) {
            case NoderVarProps::Type::Bool:
            {
                PinDecl::Ref::Bool *pin = static_cast<PinDecl::Ref::Bool *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVarProps::Type::Int:
            {
                PinDecl::Ref::Int *pin = static_cast<PinDecl::Ref::Int *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVarProps::Type::Float:
            {
                PinDecl::Ref::Float *pin = static_cast<PinDecl::Ref::Float *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVarProps::Type::String:
            {
                PinDecl::Ref::String *pin = static_cast<PinDecl::Ref::String *>(_array->list().at(i));
                _elem->sendValue(pin->value());
                break;
            }
            case NoderVarProps::Type::Enum:
            {
                //PinDecl::Ref::Enum *pin = static_cast<PinDecl::Ref::Enum *>(_array->list().at(0));
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
    */
}

void ForEachInArray::onEventConnect(void)
{
    //NoderVarProps::Type type;

    //if (_elem->valueType() != NoderVarProps::Type::Wildcard)
    //    return ;

    //type = _array->elemType();
    //deletePin(_elem);
    //_elem = addPinFromType(type, "Element", PinProperty::Direction::Output);
}

void ForEachInArray::onEventDisconnect(void)
{
    //if (_array->linked()) {
    ////    return ;
    //}
    //deletePin(_elem);
    //_elem = addPinFromType(NoderVarProps::Type::Wildcard, "Element", PinProperty::Direction::Output);
}