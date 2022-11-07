#include "NoderGraph.hpp"
#include "NoderFrame.hpp"

NoderGraph::NoderGraph(QWidget *parent)
    : PzaSplitter(Qt::Vertical, parent)
{
    _tab = new PzaTabWidget(this);

    setStretchFactor(0, 1);

    _tab->addTab(new NoderScenario(this), "Scenario");

    addWidget(_tab);
    addWidget(&Output);
}

void NoderGraph::newFunction(NoderFunction *function)
{
    int index = _tab->addTab(function, function->name());
    connect(function, &NoderFunction::nameChanged, this, [&, function](){
        _tab->setTabText(_tab->indexOf(function), function->name());
    });
    _tab->setCurrentIndex(index);
}

void NoderGraph::setActive(NoderFunction *function)
{
    int index = _tab->indexOf(function);
    _tab->setCurrentIndex(index);
}

NoderGraphicsView *NoderGraph::getGraphicView(void)
{
    NoderView *elem;

    elem = dynamic_cast<NoderView *>(_tab->currentWidget());
    if (!elem)
        return nullptr;

    return elem->view();
}