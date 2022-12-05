#include "NoderMenuBar.hpp"
#include "NoderFrame.hpp"
#include "NoderGraph.hpp"
#include "NoderSidePanel.hpp"

NoderMenuBar::NoderMenuBar(QWidget *parent)
    : PzaMenuBar(parent)
{
    PzaMenu *fileMenu = new PzaMenu("File", this);
    PzaMenu *viewMenu = new PzaMenu("View", this);
    PzaMenu *selectMenu = new PzaMenu("Select", this);
    PzaMenu *nodeMenu = new PzaMenu("Node", this);

    addActionToMenu("Save", fileMenu, [&](){
        Noder::Get().Frame->SidePanel.save();
    });

   // addActionToMenu("Load", fileMenu);
    /*

    addActionToMenu("Zoom In", viewMenu);
    addActionToMenu("Zoom Out", viewMenu);
    viewMenu->addSeparator();
    addActionToMenu("Fit to View", viewMenu);

    addActionToMenu("Select All", selectMenu);
    addActionToMenu("Deselect All", selectMenu);
    selectMenu->addSeparator();
    addActionToMenu("Select Linked Nodes", selectMenu);
    addActionToMenu("Select Same Nodes", selectMenu);
*/
    addActionToMenu("Move", nodeMenu, [&](){
        Noder::Get().Frame->Graph.getGraphicView()->doAction("move");
    });
/*    addActionToMenu("Find", nodeMenu);
    nodeMenu->addSeparator();
    addActionToMenu("Copy", nodeMenu);
    addActionToMenu("Paste", nodeMenu);
    addActionToMenu("Duplicate", nodeMenu);
    addActionToMenu("Delete", nodeMenu);
*/
    addActionToMenu("Delete", nodeMenu, [&](){
        Noder::Get().Frame->Graph.getGraphicView()->doAction("delete");
    });
/*  addActionToMenu("Unlink", nodeMenu);
    nodeMenu->addSeparator();
    addActionToMenu("Align Horizontally", nodeMenu);
    addActionToMenu("Align Vertically", nodeMenu);
*/
    addMenu(fileMenu);
    addMenu(viewMenu);
    addMenu(selectMenu);
    addMenu(nodeMenu);
}

void NoderMenuBar::addActionToMenu(const QString &name, PzaMenu *menu, const std::function<void(void)> &f)
{
    QAction *action;

    action = new QAction(name, menu);
    action->setData(QVariant::fromValue(f));
    connect(action, &QAction::triggered, this, [f]() {
        f();
    });
    
    menu->addAction(action);

    /*   QAction *action;
    
    action = new QAction(name, toMenu);
    action->setData(QVariant::fromValue(f));
    toMenu->addAction(action);
    connect(action, &QAction::triggered, this, &NoderGraphicsView::createNodeFromMenu);
*/
}