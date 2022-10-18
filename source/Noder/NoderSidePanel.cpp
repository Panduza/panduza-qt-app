#include "NoderSidePanel.hpp"

NoderSidePanel::NoderSidePanel(QWidget *parent)
    : QScrollArea(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    
    _main = new QWidget(this);
    
    _layout = new QVBoxLayout(_main);

    _varArea = new NoderVarArea(_main);
    _propertyArea = new NoderPropertyArea(_main);

    connect(_varArea, &NoderVarArea::varUpdated, _propertyArea, &NoderPropertyArea::updateProperties);
    connect(_varArea, &NoderVarArea::varAdded, _propertyArea, &NoderPropertyArea::addProperties);
    connect(_varArea, &NoderVarArea::varRemoved, _propertyArea, &NoderPropertyArea::deleteProperties);

    _layout->addWidget(_varArea);
    _layout->addWidget(_propertyArea);
    _layout->addStretch(1);

    setWidget(_main);
}

NoderVarArea::NoderVarArea(QWidget *parent)
    : PzaSpoiler("Variables", parent)
{
    setFold(false);
    
    _main = new PzaWidget(this);
    connect(_main, &PzaWidget::clicked, this, [&](){
        selectVar(nullptr);
    });
    _layout = new QVBoxLayout(_main);
    _moreLess = new PzaMoreLess("Add variable", PzaStyle::Stretch::Right, _main);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    _layout->addWidget(_moreLess);

    connect(_moreLess, &PzaMoreLess::more, this, [&]() {
        addVariable();
    });
    connect(_moreLess, &PzaMoreLess::less, this, [&]() {
        removeVariable(_selectedVar);
    });
   
    addWidget(_main);
}

void NoderVarArea::selectVar(NoderVariable *target)
{
    for (auto var : _varList) {
        var->setSelected(false);
    }
    _selectedVar = target;
    if (target)
        target->setSelected(true);
    varUpdated(_selectedVar);
}

void NoderVarArea::addVariable(void)
{
    NoderVariable *newVar = new NoderVariable(_main);

    _layout->addWidget(newVar);
    _varList.push_back(newVar);
    connect(newVar, &NoderVariable::activate, this, [&, newVar]() {
        selectVar(newVar);
    });
    varAdded(newVar);
    selectVar(newVar);
}

void NoderVarArea::removeVariable(NoderVariable *target)
{
    unsigned long index;
    NoderVariable *next = nullptr;

    if (target == nullptr)
        return ;
        
    if (_selectedVar == target) {
        _selectedVar = nullptr;
    }

    _layout->removeWidget(target);
    index = PzaUtils::IndexInVector<NoderVariable *>(_varList, target);
    PzaUtils::DeleteFromVector<NoderVariable *>(_varList, target);
    varRemoved(target);
    target->deleteLater();

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
    setFold(false);
}

void NoderPropertyArea::updateProperties(NoderVariable *var)
{
    if (var) {
        setCurrentWidget(var->propTable());
    }
}

void NoderPropertyArea::addProperties(NoderVariable *var)
{
    addWidget(var->propTable());
}

void NoderPropertyArea::deleteProperties(NoderVariable *var)
{
    removeWidget(var->propTable());
}