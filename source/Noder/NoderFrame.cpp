#include "NoderFrame.hpp"

NoderFrame::NoderFrame()
    : PzaWidget()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    PzaSplitter *mainFrame = new PzaSplitter(Qt::Horizontal, this);
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 5);
    _layout->setSpacing(0);

    mainFrame->addWidget(&SidePanel);
    mainFrame->addWidget(&Graph);
    mainFrame->setStretchFactor(1, 1);

    _layout->addWidget(&MenuBar);
    _layout->addWidget(mainFrame);
}