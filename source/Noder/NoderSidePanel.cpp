#include "NoderSidePanel.hpp"

NoderSidePanel::NoderSidePanel(QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    _scenarioArea = new NoderScenarioArea(_main);
    _varArea = new NoderVariableArea(_main);

    _layout->addWidget(_scenarioArea);
    _layout->addWidget(_varArea);
    _layout->addStretch(1);

    setStyleSheet("background-color: #252525");

    setWidget(_main);
}

NoderVariableArea::NoderVariableArea(QWidget *parent)
    : PzaSpoiler("Variables", parent)
{
    _main = new PzaWidget(this);
    _varTable = new PzaWidget(_main);
    _varTableLayout = new QVBoxLayout(_varTable);
    _propertyArea = new NoderPropertyArea(_main);
    _defValArea = new NoderDefValArea(_main);

    _varTableLayout->setContentsMargins(3, 3, 3, 3);
    _varTableLayout->setSpacing(0);

    connect(_main, &PzaWidget::clicked, this, [&](){
        selectVar(nullptr);
    });
    _layout = new QVBoxLayout(_main);
    _moreLess = new PzaMoreLess("Add variable", _main);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    _layout->addWidget(_moreLess);
    _layout->addWidget(_varTable);
    _layout->addWidget(_propertyArea);
    _layout->addWidget(_defValArea);

    connect(_moreLess, &PzaMoreLess::more, this, &NoderVariableArea::addVariable);
    connect(_moreLess, &PzaMoreLess::less, this, &NoderVariableArea::removeVariable);
   
    addWidget(_main);
}

void NoderVariableArea::selectVar(NoderVariable *target)
{
    for (auto var : _varList) {
        var->setSelected(false);
    }
    _selectedVar = target;
    if (target)
        target->setSelected(true);
    _propertyArea->updateProperty(_selectedVar);
    _defValArea->updateValArea(_selectedVar);
}

void NoderVariableArea::addVariable(void)
{
    NoderVariable *newVar;
    NoderPanel::Type fromType;

    fromType = (_selectedVar) ? _selectedVar->type() : NoderPanel::Type::Bool;

    newVar = new NoderVariable(fromType, _main);

    _varTableLayout->addWidget(newVar);
    _varList.push_back(newVar);
    connect(newVar, &NoderVariable::activate, this, [&, newVar]() {
        selectVar(newVar);
    });
    connect(newVar, &NoderVariable::typeChanged, this, [&, newVar]() {
        if (newVar->defValTable())
            _defValArea->del(newVar);
        newVar->createDefValTable();
        _defValArea->add(newVar);
    });
    _propertyArea->addProperty(newVar);
    _defValArea->add(newVar);
    selectVar(newVar);
}

void NoderVariableArea::removeVariable(void)
{
    unsigned long index;
    NoderVariable *next = nullptr;

    if (_selectedVar == nullptr)
        return ;
        
    _varTableLayout->removeWidget(_selectedVar);
    index = PzaUtils::IndexInVector<NoderVariable *>(_varList, _selectedVar);
    PzaUtils::DeleteFromVector<NoderVariable *>(_varList, _selectedVar);
    _propertyArea->deleteProperty(_selectedVar);
    _defValArea->del(_selectedVar);
    _selectedVar->deleteLater();

    if (index != _varList.size()) {
        next = _varList.at(index);
    }
    else if (_varList.size() > 0) {
        next = _varList.at(index - 1);
    }
    selectVar(next);
}

NoderPropertyArea::NoderPropertyArea(QWidget *parent)
    : PzaSpoiler("Properties", parent)
{
    
}

void NoderPropertyArea::updateProperty(NoderVariable *var)
{
    if (var)
        setCurrentWidget(var->propTable());
}

void NoderPropertyArea::addProperty(NoderVariable *var)
{
    addWidget(var->propTable());
}

void NoderPropertyArea::deleteProperty(NoderVariable *var)
{
    removeWidget(var->propTable());
}

NoderDefValArea::NoderDefValArea(QWidget *parent)
    : PzaSpoiler("Values", parent)
{

}

void NoderDefValArea::updateValArea(NoderVariable *var)
{
    if (var)
        setCurrentWidget(var->defValTable());
}

void NoderDefValArea::add(NoderVariable *var)
{
    addWidget(var->defValTable());
}

void NoderDefValArea::del(NoderVariable *var)
{
    removeWidget(var->defValTable());
}

NoderScenarioArea::NoderScenarioArea(QWidget *parent)
    : PzaSpoiler("Scenarios", parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);

    addWidget(_main);
}