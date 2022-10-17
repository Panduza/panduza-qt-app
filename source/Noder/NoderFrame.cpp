#include "NoderFrame.hpp"

NoderFrame::NoderFrame(PanduzaEngine *engine)
    : QWidget(),
    _engine(engine)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    PzaMenuBar *menuBar = mainMenuBar();

    NoderView *view = new NoderView(engine);

    //connect(engine, &PanduzaEngine::interfaceRegistered, this, &NoderFrame::newInterface);

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 5);
    _mainLayout->setSpacing(0);

    QTabWidget *tabBar = new QTabWidget(this);

    tabBar->addTab(view, "Scenario 1");

    QSplitter *graphFrame = new QSplitter(Qt::Horizontal, this);
    graphFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphFrame->setContentsMargins(0, 0, 0, 0);
    graphFrame->setStyleSheet("background-color: #181818;");

    NoderSidePanel *sidePanel = new NoderSidePanel(this);

    graphFrame->addWidget(sidePanel);
    graphFrame->addWidget(tabBar);

    graphFrame->setStretchFactor(1, 1);
    _mainLayout->addWidget(menuBar);
    _mainLayout->addWidget(graphFrame);
}

/*
void Lol::mouseMoveEvent(QMouseEvent *event)
{
    QDrag *drag = new QDrag(this);

     QMimeData *mimeData = new QMimeData;

    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec();
}
*/
PzaMenuBar *NoderFrame::mainMenuBar(void)
{
    PzaMenuBar *menuBar = new PzaMenuBar();

    PzaMenu *menuView = new PzaMenu("View", menuBar);
    PzaMenu *menuSelect = new PzaMenu("Select", menuBar);
    PzaMenu *menuNode = new PzaMenu("Node", menuBar);

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

    menuBar->addMenu(menuView);
    menuBar->addMenu(menuSelect);
    menuBar->addMenu(menuNode);

    return menuBar;
}

NoderFrame::~NoderFrame()
{
    
}