#include <NoderDataModel.hpp>
#include <PzaMenu.hpp>

NoderDataModel::NoderDataModel(PanduzaEngine *engine)
    : _engine(engine)
{
    initNodeMenuList();
}

void NoderDataModel::initNodeMenuList(void)
{
    PzaMenu *EventMenu = new PzaMenu("Event");
    PzaMenu *BranchMenu = new PzaMenu("Branch");
    PzaMenu *ArithMenu = new PzaMenu("Arithmetic");
    PzaMenu *StringMenu = new PzaMenu("String");
    PzaMenu *TimeMenu = new PzaMenu("Time");
    PzaMenu *InterfaceMenu = new PzaMenu("Interfaces");

    _nodeMenuList.push_back(EventMenu);
    _nodeMenuList.push_back(BranchMenu);
    _nodeMenuList.push_back(ArithMenu);
    _nodeMenuList.push_back(StringMenu);
    _nodeMenuList.push_back(TimeMenu);
    _nodeMenuList.push_back(InterfaceMenu);

    PzaMenu *InterfaceCommonMenu = new PzaMenu("Interface");
    PzaMenu *IoMenu = new PzaMenu("Io");
    PzaMenu *PsuMenu = new PzaMenu("Power Supply");

    InterfaceMenu->addMenu(InterfaceCommonMenu);
    InterfaceMenu->addMenu(IoMenu);
    InterfaceMenu->addMenu(PsuMenu);

    AddNodeToMenu(EventMenu, "Event Start", Node::createChildNode<N_Event_Start>);
    
    AddNodeToMenu(BranchMenu, "If", Node::createChildNode<N_Branch_If>);
    AddNodeToMenu(BranchMenu, "For", Node::createChildNode<N_Branch_For>);
    
    AddNodeToMenu(ArithMenu, "Math", Node::createChildNode<N_Arithmetic_Math>);

    AddNodeToMenu(StringMenu, "Print String", Node::createChildNode<N_String_Print>);
    AddNodeToMenu(StringMenu, "Append String", Node::createChildNode<N_String_Append>);
    
    AddNodeToMenu(TimeMenu, "Delay", Node::createChildNode<N_Time_Delay>);

    AddNodeToMenu(InterfaceCommonMenu, "Find Interface", Node::createChildNode<N_Interface_Find>);

    AddNodeToMenu(IoMenu, "Initialize I/O", Node::createChildNode<N_Io_Init>);
    AddNodeToMenu(IoMenu, "Set I/O Value", Node::createChildNode<N_Io_Direction>);
    AddNodeToMenu(IoMenu, "Set I/O Direction", Node::createChildNode<N_Io_Value>);
}

void NoderDataModel::AddNodeToMenu(PzaMenu *toMenu, const QString &name, t_createChildNode f)
{
    QAction *action;
    
    action = new QAction(name);
    action->setData(QVariant::fromValue(f));
    toMenu->addAction(action);
}