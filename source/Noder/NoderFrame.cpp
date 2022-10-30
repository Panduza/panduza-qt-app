#include "NoderFrame.hpp"
#include "NoderSidePanel.hpp"
#include "NoderGraph.hpp"
#include "NoderMenuBar.hpp"

NoderFrame::NoderFrame()
    : PzaWidget()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    SidePanel = NoderSidePanel::Get();
    Graph = new NoderGraph(this);
    MenuBar = new NoderMenuBar(this);

    PzaSplitter *mainFrame = new PzaSplitter(Qt::Horizontal, this);
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 5);
    _layout->setSpacing(0);

    mainFrame->setStyleSheet("background-color: #181818;");

    mainFrame->addWidget(SidePanel);
    mainFrame->addWidget(Graph);
    mainFrame->setStretchFactor(1, 1);

    _layout->addWidget(MenuBar);
    _layout->addWidget(mainFrame);
}