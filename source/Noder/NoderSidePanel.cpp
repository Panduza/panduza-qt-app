#include "NoderSidePanel.hpp"

NoderSidePanel::NoderSidePanel(QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    FunctionArea = new NoderFunctionArea(_main);
    VariableArea = new NoderVariableArea(_main);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    _layout->addWidget(FunctionArea);
    _layout->addWidget(VariableArea);
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
    _selectedVar->dead();
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

NoderFunctionProperties::NoderFunctionProperties(QWidget *parent)
    : PzaPopup(parent)
{

}

NoderFunctionEntry::NoderFunctionEntry(const QString &name, QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QHBoxLayout(this);
    _deleteBtn = new PzaPushButton(this);
    _propBtn = new PzaPushButton(this);
    _name = new PzaLabel(this);
    _propTable = new PzaPropertyTable(this);
    _propDialog = new PzaPopup("Edit Function", _propTable, this);
    _function = new NoderFunction();

    _propDialog->setValidator();

    _propName = _propTable->addProperty<PzaLineEdit>("Name");
    _propName->setText(name);
    _function->setName(name);

    connect(_propDialog, &PzaPopup::validated, this, [&]() {
        setName(_propName->text());
    });

    connect(_deleteBtn, &PzaPushButton::clicked, [&](){removed();});

    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(5);

    _propBtn->setFixedSize(20, 20);
    _deleteBtn->setFixedSize(20, 20);

    _propBtn->setMenu(_propDialog);

    _layout->addWidget(_name);
    _layout->addWidget(_propBtn);
    _layout->addWidget(_deleteBtn);

    setFocusProxy(_name);

    NoderFrame::Get()->Graph->newFunction(_function);
}

NoderFunctionEntry::~NoderFunctionEntry()
{
    _function->deleteLater();
}

NoderFunctionArea::NoderFunctionArea(QWidget *parent)
    : PzaSpoiler("Function", parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    _functionTable = new PzaWidget(_main);
    _functionTableLayout = new QVBoxLayout(_functionTable);

    PzaPushButton *addBtn = new PzaPushButton(_main);

    addBtn->setText("Add function");
    connect(addBtn, &PzaPushButton::clicked, this, &NoderFunctionArea::addFunction);

    _layout->addWidget(addBtn);
    _layout->addWidget(_functionTable);

    addWidget(_main);
}

void NoderFunctionArea::addFunction(void)
{
    auto getNameInVector = [](NoderFunctionEntry *entry) -> const QString & {
        return entry->function()->name();
    };

    const QString &name = PzaUtils::allocateName<NoderFunctionEntry *>(_functionList, DEFAULT_FUNCTION_NAME, getNameInVector);

    NoderFunctionEntry *entry = new NoderFunctionEntry(name, this);

    entry->setName(name);

    connect(entry, &NoderFunctionEntry::removed, this, &NoderFunctionArea::removeFunction);

    _functionList.push_back(entry);
    _functionTableLayout->addWidget(entry);
}

void NoderFunctionArea::removeFunction(void)
{
    NoderFunctionEntry *entry = static_cast<NoderFunctionEntry *>(sender());

    PzaUtils::DeleteFromVector(_functionList, entry);
    entry->deleteLater();
}