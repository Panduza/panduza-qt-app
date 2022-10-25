#include <NoderDataBase.hpp>
#include <Pin.hpp>
#include <PanduzaEngine.hpp>
#include <GNode.hpp>

#include <NFArray.hpp>
#include <NFMath.hpp>
#include <NFString.hpp>
#include <NFFor.hpp>
#include <NFIf.hpp>
#include <NFEvent.hpp>
#include <NFDelay.hpp>
#include <NFIO.hpp>

NoderDataBase::NoderDataBase()
{
    initNodeMenuList();
}

void NoderDataBase::initNodeMenuList(void)
{
    auto AddNodeToMenu = [](PzaMenu *toMenu, const QString &name, const t_CreateNode &f)
    {
        QAction *action;
    
        action = new QAction(name);
        action->setData(QVariant::fromValue(f));
        toMenu->addAction(action);
    };

    PzaMenu *EventMenu = new PzaMenu("Event");
    PzaMenu *BranchMenu = new PzaMenu("Branch");
    PzaMenu *ArithMenu = new PzaMenu("Arithmetic");
    PzaMenu *StringMenu = new PzaMenu("String");
    PzaMenu *TimeMenu = new PzaMenu("Time");
    PzaMenu *ArrayMenu = new PzaMenu("Array");
    PzaMenu *InterfaceMenu = new PzaMenu("Interfaces");

    _nodeMenuList.push_back(EventMenu);
    _nodeMenuList.push_back(BranchMenu);
    _nodeMenuList.push_back(ArithMenu);
    _nodeMenuList.push_back(StringMenu);
    _nodeMenuList.push_back(TimeMenu);
    _nodeMenuList.push_back(ArrayMenu);
    _nodeMenuList.push_back(InterfaceMenu);

    PzaMenu *InterfaceCommonMenu = new PzaMenu("Interface");
    PzaMenu *IoMenu = new PzaMenu("Io");
    PzaMenu *PsuMenu = new PzaMenu("Power Supply");

    InterfaceMenu->addMenu(InterfaceCommonMenu);
    InterfaceMenu->addMenu(IoMenu);
    InterfaceMenu->addMenu(PsuMenu);

    AddNodeToMenu(EventMenu, "Event Start", GNode::CreateNode<MainEvent>);
    
    AddNodeToMenu(BranchMenu, "If", GNode::CreateNode<If>);
    AddNodeToMenu(BranchMenu, "For", GNode::CreateNode<For>);
    AddNodeToMenu(BranchMenu, "For Each Element in Array", GNode::CreateNode<ForEachInArray>);
    
    AddNodeToMenu(ArithMenu, "Add", GNode::CreateNode<Addition>);
    AddNodeToMenu(ArithMenu, "Substract", GNode::CreateNode<Substraction>);
    AddNodeToMenu(ArithMenu, "Multiply", GNode::CreateNode<Multiply>);
    AddNodeToMenu(ArithMenu, "Divide", GNode::CreateNode<Divide>);
    AddNodeToMenu(ArithMenu, "Modulo", GNode::CreateNode<Modulo>);

    AddNodeToMenu(StringMenu, "Print String", GNode::CreateNode<Print>);
    AddNodeToMenu(StringMenu, "Append String", GNode::CreateNode<Cat>);
    
    AddNodeToMenu(TimeMenu, "Delay", GNode::CreateNode<Delay>);
    
    AddNodeToMenu(ArrayMenu, "Array", GNode::CreateNode<Array>);

    AddNodeToMenu(InterfaceCommonMenu, "Find Interface", GNode::CreateNode<Addition>);

    AddNodeToMenu(IoMenu, "Initialize I/O", GNode::CreateNode<IoInit>);
    AddNodeToMenu(IoMenu, "Set I/O Value", GNode::CreateNode<Addition>);
    AddNodeToMenu(IoMenu, "Set I/O Direction", GNode::CreateNode<Addition>);
}

void NoderDataBase::forEachEnum(std::function<void(const QString &name, const std::vector<QString> &list)> func)
{
    for (auto var : _enumMap) {
        func(var.first, var.second);
    }
}

void NoderDataBase::forEachEnumName(std::function<void(const QString &name)> func)
{
    for (auto var : _enumMap) {
        func(var.first);
    }
}

void NoderDataBase::forEachEnumValues(const QString &name, std::function<void(const QString &name)> func)
{
    for (auto var : _enumMap[name]) {
        func(var);
    }
}

const std::vector<QString> &NoderDataBase::enumValues(const QString &name)
{
    return _enumMap[name];
}

Pin *NoderDataBase::pinTypeToObj(const PinProperty::Type type)
{
    switch (type) {
        case PinProperty::Type::Bool:       return new PinDecl::Bool();
        case PinProperty::Type::Float:      return new PinDecl::Float();
        case PinProperty::Type::Int:        return new PinDecl::Int();
        case PinProperty::Type::String:     return new PinDecl::String();
        case PinProperty::Type::Wildcard:   return new PinDecl::Wildcard();
        case PinProperty::Type::Enum:       return new PinDecl::Enum();
        case PinProperty::Type::Array:      return new PinDecl::Array();
        case PinProperty::Type::Interface:  return new PinDecl::Interface();
        default:                            return nullptr;
    }
}

PinProperty::Type NoderDataBase::panelTypeToPinType(const NoderPanel::Type type)
{
    switch (type) {
        case NoderPanel::Type::Bool:    return PinProperty::Type::Bool;
        case NoderPanel::Type::Int:     return PinProperty::Type::Int;
        case NoderPanel::Type::Float:   return PinProperty::Type::Float;
        case NoderPanel::Type::String:  return PinProperty::Type::String;
        case NoderPanel::Type::Enum:    return PinProperty::Type::Enum;
        default:                        return PinProperty::Type::Bool;
    }
}

const QString &NoderDataBase::pinTypeToStr(const PinProperty::Type type)
{
    static std::unordered_map<PinProperty::Type, QString> map = {
        {PinProperty::Type::Wildcard, "Wildcard"},
        {PinProperty::Type::Int, "Integer"},
        {PinProperty::Type::Float, "Float"},
        {PinProperty::Type::Bool, "Boolean"},
        {PinProperty::Type::String, "String"},
        {PinProperty::Type::Enum, "Enum"},
        {PinProperty::Type::Array, "Wildcard"},
        {PinProperty::Type::Interface, "Interface"}
    };
    return map[type];
}

const QString &NoderDataBase::pinTypeToDir(const PinProperty::Direction direction)
{
    static std::unordered_map<PinProperty::Direction, QString> map = {
        {PinProperty::Direction::Input, "Input"},
        {PinProperty::Direction::Output, "Output"}
    };
    return map[direction];
}

const QColor &NoderDataBase::plugColor(const PinProperty::Type type)
{
    static std::unordered_map<PinProperty::Type, QColor> map = {
        {PinProperty::Type::Wildcard, QColor("white")},
        {PinProperty::Type::Bool, QColor("#E20909")},
        {PinProperty::Type::Int, QColor("#19C5CC")},
        {PinProperty::Type::Float, QColor("#3AC242")},
        {PinProperty::Type::String, QColor("#C05DC2")},
        {PinProperty::Type::Enum, QColor("#CE6135")},
        {PinProperty::Type::Interface, QColor("#C8B623")},
    };
    return map[type];
}

const QColor &NoderDataBase::varColor(const NoderPanel::Type type)
{
    static std::unordered_map<NoderPanel::Type, QColor> map = {
        {NoderPanel::Type::Bool, plugColor(PinProperty::Type::Bool)},
        {NoderPanel::Type::Int,  plugColor(PinProperty::Type::Int)},
        {NoderPanel::Type::Float, plugColor(PinProperty::Type::Float)},
        {NoderPanel::Type::String, plugColor(PinProperty::Type::String)},
        {NoderPanel::Type::Enum,  plugColor(PinProperty::Type::Enum)},
    };
    return map[type];
}

const QString &NoderDataBase::nodeTypeName(const NodeProperty::Type type)
{
    static std::unordered_map<NodeProperty::Type, QString> map = {
        {NodeProperty::Type::Operation, "Operation"},
        {NodeProperty::Type::Exec, "Execution"},
        {NodeProperty::Type::Branch, "Branch"},
        {NodeProperty::Type::Instance, "Instance"},
        {NodeProperty::Type::Event , "Event"}
    };
    return map[type];
}

const QString &NoderDataBase::varTypeName(const NoderPanel::Type type)
{
    return _varTypeMap[type];
}

NoderPanel::Type NoderDataBase::varTypeFromName(const QString &name)
{
    for (const auto& [key, value] : _varTypeMap)
        if (value == name)
            return key;
    // Should not be possible
    return NoderPanel::Type::Bool;
}

void NoderDataBase::forEachVarType(std::function<void(NoderPanel::Type type)> func)
{
    for (auto var : _varTypeMap) {
        func(var.first);
    }
}
