#include "NoderGraphArea.hpp"

NoderGraphArea::NoderGraphArea(QWidget *parent)
    : PzaSplitter(Qt::Vertical, parent)
{
    _tab = new PzaTabWidget(this);
    _outputArea = new NoderOutputArea(this);

    setStretchFactor(0, 1);

    _tab->addTab(new NoderMainEvent(this), "Main Event");

    addWidget(_tab);
    addWidget(_outputArea);
}

void NoderGraphArea::newScenario(NoderScenario *scenario)
{
    _tab->addTab(scenario, scenario->name());
    connect(scenario, &NoderScenario::nameChanged, this, [&, scenario](){
        _tab->setTabText(_tab->indexOf(scenario), scenario->name());
    });
}

void NoderGraphArea::newFunction(NoderFunction *function)
{
    _tab->addTab(function, function->name());
    connect(function, &NoderFunction::nameChanged, this, [&, function](){
        _tab->setTabText(_tab->indexOf(function), function->name());
    });
}