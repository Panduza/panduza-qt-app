#include "NoderMenuBar.hpp"

NoderMenuBar::NoderMenuBar(QWidget *parent)
    : PzaMenuBar(parent)
{
    PzaMenu *menuView = new PzaMenu("View", this);
    PzaMenu *menuSelect = new PzaMenu("Select", this);
    PzaMenu *menuNode = new PzaMenu("Node", this);

    QAction *viewZoomIn = new QAction("Zoom In", menuView);
    QAction *viewZoomOut = new QAction("Zoom Out", menuView);
    QAction *viewFit = new QAction("Fit to View", menuView);

    QAction *selectAll = new QAction("Select All", menuSelect);
    QAction *deselectAll = new QAction("Deselect All", menuSelect);
    QAction *selectLinked = new QAction("Select Linked Nodes", menuSelect);
    QAction *selectSame = new QAction("Select Same Nodes", menuSelect);

    QAction *nodeAdd = new QAction("Add", menuNode);
    QAction *nodeMove = new QAction("Move", menuNode);
    QAction *nodeFind = new QAction("Find", menuNode);
    QAction *nodeCopy = new QAction("Copy", menuNode);
    QAction *nodePaste = new QAction("Paste", menuNode);
    QAction *nodeDuplicate = new QAction("Duplicate", menuNode);
    QAction *nodeDelete = new QAction("Delete", menuNode);
    QAction *nodeUnlink = new QAction("Unlink", menuNode);
    QAction *nodeAlignH = new QAction("Align Horizontally", menuNode);
    QAction *nodeAlignV = new QAction("Align Vertically", menuNode);

    menuView->addAction(viewZoomIn);
    menuView->addAction(viewZoomOut);
    menuView->addSeparator();
    menuView->addAction(viewFit);

    menuSelect->addAction(selectAll);
    menuSelect->addAction(deselectAll);
    menuSelect->addSeparator();
    menuSelect->addAction(selectLinked);
    menuSelect->addAction(selectSame);

    menuNode->addAction(nodeAdd);
    menuNode->addAction(nodeMove);
    menuNode->addAction(nodeFind);
    menuNode->addSeparator();
    menuNode->addAction(nodeCopy);
    menuNode->addAction(nodePaste);
    menuNode->addAction(nodeDuplicate);
    menuNode->addAction(nodeDelete);
    menuNode->addSeparator();
    menuNode->addAction(nodeUnlink);
    menuNode->addSeparator();
    menuNode->addAction(nodeAlignH);
    menuNode->addAction(nodeAlignV);

    addMenu(menuView);
    addMenu(menuSelect);
    addMenu(menuNode);
}