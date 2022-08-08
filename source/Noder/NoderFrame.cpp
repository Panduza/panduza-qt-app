#include <NoderFrame.hpp>

NoderFrame::NoderFrame(PanduzaEngine *engine)
    : QWidget(),
    _engine(engine)
{
    PzaMenuBar *menuBar = mainMenuBar();

    NoderDataModel *dataModel = new NoderDataModel(engine);
    NoderView *view = new NoderView(engine, dataModel);

    connect(engine, &PanduzaEngine::interfaceRegistered, this, &NoderFrame::newInterface);

    _main_layout = new QVBoxLayout(this);
    _main_layout->setContentsMargins(0, 0, 0, 5);
    _main_layout->setSpacing(0);

    QTabWidget *tabBar = new QTabWidget(this);

    tabBar->addTab(view, "Scenario 1");

    QWidget *sidePanel = new QWidget(this);
    _sidePanelLayout = new QVBoxLayout(sidePanel);
    _sidePanelLayout->setContentsMargins(0, 0, 0, 0);
    _sidePanelLayout->setSpacing(0);

    QSplitter *graphFrame = new QSplitter(Qt::Horizontal, this);
    graphFrame->setContentsMargins(0, 0, 0, 0);
    graphFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    graphFrame->setStyleSheet("background-color: #181818;");

    graphFrame->addWidget(sidePanel);
    graphFrame->addWidget(tabBar);
    graphFrame->addWidget(new QFrame());

    graphFrame->setSizes(QList<int>({30, 500, 30}));

    _main_layout->addWidget(menuBar);
    _main_layout->addWidget(graphFrame);
}

void NoderFrame::newInterface(Interface *interface)
{
    QLabel *label = new QLabel(interface->name());

    label->setStyleSheet("color: white;");

    _sidePanelLayout->addWidget(label);
}

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