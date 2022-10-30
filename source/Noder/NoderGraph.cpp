#include "NoderGraph.hpp"

NoderGraph::NoderGraph(QWidget *parent)
    : PzaSplitter(Qt::Vertical, parent)
{
    _tab = new PzaTabWidget(this);
    _outputArea = new NoderOutputArea(this);

    setStretchFactor(0, 1);

    _tab->addTab(new NoderScenario(this), "Scenario");

    addWidget(_tab);
    addWidget(_outputArea);
}

void NoderGraph::newFunction(NoderFunction *function)
{
    int index = _tab->addTab(function, function->name());
    connect(function, &NoderFunction::nameChanged, this, [&, function](){
        _tab->setTabText(_tab->indexOf(function), function->name());
    });
    _tab->setCurrentIndex(index);
}
/*
void NoderGraph::removeFunction(NoderFunction *function)
{
    int index = _tab->addTab(function, function->name());
    connect(function, &NoderFunction::nameChanged, this, [&, function](){
        _tab->setTabText(_tab->indexOf(function), function->name());
    });
    _tab->setCurrentIndex(index);
}

*/