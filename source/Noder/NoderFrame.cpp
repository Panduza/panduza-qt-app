#include "NoderFrame.hpp"

QWidget *sidePanel(QWidget *parent);

NoderFrame::NoderFrame(PanduzaEngine *engine)
    : QWidget(),
    _engine(engine)
{
    PzaMenuBar *menuBar = mainMenuBar();

    NoderView *view = new NoderView(engine);

    //connect(engine, &PanduzaEngine::interfaceRegistered, this, &NoderFrame::newInterface);

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 5);
    _mainLayout->setSpacing(0);

    QTabWidget *tabBar = new QTabWidget(this);

    tabBar->addTab(view, "Scenario 1");
    
    QSplitter *graphFrame = new QSplitter(Qt::Horizontal, this);
    graphFrame->setContentsMargins(0, 0, 0, 0);
    graphFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    graphFrame->setStyleSheet("background-color: #181818;");

    graphFrame->addWidget(sidePanel(this));
    graphFrame->addWidget(tabBar);
    graphFrame->addWidget(new QFrame());

    graphFrame->setSizes(QList<int>({30, 500, 30}));

    _mainLayout->addWidget(menuBar);
    _mainLayout->addWidget(graphFrame);

}

#include <QSpinBox>

class Lol : public QWidget
{
    public:
        Lol(QWidget *parent);
        ~Lol() = default;

        void mouseMoveEvent(QMouseEvent *event);
};

Lol::Lol(QWidget *parent)
    : QWidget(parent)
{
    
}

#include <QDrag>
#include <QMimeData>

void Lol::mouseMoveEvent(QMouseEvent *event)
{
    QDrag *drag = new QDrag(this);

     QMimeData *mimeData = new QMimeData;

    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec();
}

QWidget *varTab(QWidget *parent)
{
    Lol *main = new Lol(parent);
    QHBoxLayout *layout = new QHBoxLayout(main);
    QLabel *name = new QLabel("Name", main);
    QLabel *VarColor = new QLabel(main);
    
    name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    VarColor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QSpinBox *valuebox = new QSpinBox(main);
    valuebox->setStyleSheet("QSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}");
    valuebox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    main->setStyleSheet("background-color:blue");

    name->setStyleSheet("background-color: black;color: white");

    VarColor->setFixedSize(15, 10);

    VarColor->setStyleSheet(
        "background-color: red;"
        "border: 1px solid gray;"
        "border-radius: 4px;"
    );

    layout->addWidget(name);
    layout->addWidget(VarColor);
    layout->addWidget(valuebox);

    return main;
}

QWidget *sidePanel(QWidget *parent)
{
    QWidget *sidePanel = new QWidget(parent);
    QVBoxLayout *sidePanelLayout = new QVBoxLayout(sidePanel);

    sidePanel->setStyleSheet("background-color: red");

    QWidget *varContainer = new QWidget(sidePanel);
    QVBoxLayout *varContainerLayout = new QVBoxLayout(varContainer);

    varContainer->setStyleSheet("background-color: yellow");

    QLabel *varContainerTitle = new QLabel("Variables", varContainer);

    varContainerLayout->addWidget(varContainerTitle);
    varContainerLayout->addWidget(varTab(varContainerTitle));
    varContainerLayout->addWidget(varTab(varContainerTitle));
    varContainerLayout->addWidget(varTab(varContainerTitle));
    varContainerLayout->addWidget(varTab(varContainerTitle));
    varContainerLayout->addWidget(varTab(varContainerTitle));
    varContainerLayout->addWidget(varTab(varContainerTitle));

    varContainerLayout->addStretch(1);

    sidePanelLayout->addWidget(varContainer);

    sidePanelLayout->addStretch(1);

    sidePanelLayout->setContentsMargins(0, 0, 0, 0);
    sidePanelLayout->setSpacing(0);

    return sidePanel;
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