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

    connect(_varArea, &NoderVarArea::updated, _propertyArea, &NoderPropertyArea::updateProperties);

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

    setContentWidget(_main);

    _layout->addWidget(_moreLess);

    connect(_moreLess, &PzaMoreLess::more, this, [&]() {
        addVariable();
    });
    connect(_moreLess, &PzaMoreLess::less, this, [&]() {
        removeVariable(_selectedVar);
    });
}

void NoderVarArea::selectVar(NoderVariable *target)
{
    for (auto var : _varList) {
        var->setSelected(false);
    }
    _selectedVar = target;
    if (target) {
        target->setSelected(true);
        updated(_selectedVar);
    }
}

void NoderVarArea::addVariable(void)
{
    NoderVariable *newVar = new NoderVariable(_main);

    _layout->addWidget(newVar);
    _varList.push_back(newVar);
    connect(newVar, &NoderVariable::activate, this, [&, newVar]() {
        selectVar(newVar);
    });
    connect(newVar, &NoderVariable::nameChanged, this, [&, newVar]() {
        updated(_selectedVar);
    });
   // selectVar(newVar);
}

void NoderVarArea::removeVariable(NoderVariable *target)
{
    if (target == nullptr)
        return ;
        
    if (_selectedVar == target) {
        _selectedVar = nullptr;
    }

    _layout->removeWidget(target);
    PzaUtils::DeleteFromVector<NoderVariable *>(_varList, target);
    delete target;
}

NoderPropertyArea::NoderPropertyArea(QWidget *parent)
    : PzaSpoiler("Properties", parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    _propertyTable = new PzaPropertyTable(this);
    _propName = _propertyTable->addProperty<PzaLabel>("Name");
    _propType = _propertyTable->addProperty<PzaComboBox>("Type");

    NBD_INST.forEachVarType([&](NoderPanel::Type type) {
        _propType->insertItem(0, NBD_INST.varTypeName(type));
    });
    _propType->setCurrentIndex(0);

    setContentWidget(_propertyTable);
}

void NoderPropertyArea::updateProperties(NoderVariable *var)
{
    if (folded())
        setFold(false);
    _propName->setText(var->name());
}