#include "NoderFunction.hpp"
#include "NoderVariable.hpp"

NoderFunction::NoderFunction(QWidget *parent)
    : NoderView(parent)
{
    PzaMenu *menu = new PzaMenu(this);

    PzaMenu *BranchMenu = new PzaMenu("Branch", menu);
    PzaMenu *ArithMenu = new PzaMenu("Arithmetic", menu);
    PzaMenu *StringMenu = new PzaMenu("String", menu);
    PzaMenu *TimeMenu = new PzaMenu("Time", menu);
    PzaMenu *ArrayMenu = new PzaMenu("Array", menu);
    PzaMenu *InterfaceMenu = new PzaMenu("Interfaces", menu);

    PzaMenu *InterfaceCommonMenu = new PzaMenu("Interface", menu);
    PzaMenu *IoMenu = new PzaMenu("Io", menu);
    PzaMenu *PsuMenu = new PzaMenu("Power Supply", menu);
 
    menu->addMenu(BranchMenu);
    menu->addMenu(ArithMenu);
    menu->addMenu(StringMenu);
    menu->addMenu(TimeMenu);
    menu->addMenu(ArrayMenu);
    menu->addMenu(InterfaceMenu);
    
    InterfaceMenu->addMenu(InterfaceCommonMenu);
    InterfaceMenu->addMenu(IoMenu);
    InterfaceMenu->addMenu(PsuMenu);

    view()->addNodeToMenu(BranchMenu, "Mdr", GNode::CreateNode<EventFunctionFinal>);
    view()->addNodeToMenu(BranchMenu, "If", GNode::CreateNode<If>);
    view()->addNodeToMenu(BranchMenu, "For", GNode::CreateNode<For>);
    view()->addNodeToMenu(BranchMenu, "For Each Element in Array", GNode::CreateNode<ForEachInArray>);
    view()->addNodeToMenu(ArithMenu, "Add", GNode::CreateNode<Addition>);
    view()->addNodeToMenu(ArithMenu, "Substract", GNode::CreateNode<Substraction>);
    view()->addNodeToMenu(ArithMenu, "Multiply", GNode::CreateNode<Multiply>);
    view()->addNodeToMenu(ArithMenu, "Divide", GNode::CreateNode<Divide>);
    view()->addNodeToMenu(ArithMenu, "Modulo", GNode::CreateNode<Modulo>);
    view()->addNodeToMenu(StringMenu, "Print String", GNode::CreateNode<Print>);
    view()->addNodeToMenu(StringMenu, "Append String", GNode::CreateNode<Cat>);
    view()->addNodeToMenu(TimeMenu, "Delay", GNode::CreateNode<Delay>);
    view()->addNodeToMenu(ArrayMenu, "Array", GNode::CreateNode<Array>);
    view()->addNodeToMenu(InterfaceCommonMenu, "Find Interface", GNode::CreateNode<Addition>);
    view()->addNodeToMenu(IoMenu, "Initialize I/O", GNode::CreateNode<IoInit>);
    view()->addNodeToMenu(IoMenu, "Set I/O Value", GNode::CreateNode<Addition>);
    view()->addNodeToMenu(IoMenu, "Set I/O Direction", GNode::CreateNode<Addition>);

    view()->setMenu(menu);

    _startNode = view()->createNode(GNode::CreateNode<EventFunctionStart>, view()->mapToScene(view()->rect().center()));
    _endNode = view()->createNode(GNode::CreateNode<EventFunctionEnd>, view()->rect().center());
}