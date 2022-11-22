#include "NoderSPVariable.hpp"

NoderSPVariableEntry::NoderSPVariableEntry(QWidget *parent)
    : NoderSPEntry<PzaWidget>(parent)
{
    _varDrop = new NoderSPVarDrop(this);

    connect(_varDrop, &NoderSPVarDrop::varChanged, this, [&](NoderVar::Container ctn, NoderVar::Type type) {
        _ctn = ctn;
        _type = type;
    });

    _layout->insertWidget(1, _varDrop);
}

void NoderSPVariableEntry::setContainer(NoderVar::Container ctn)
{
    _ctn = ctn;
    _varDrop->setContainer(ctn);
}

void NoderSPVariableEntry::setType(NoderVar::Type type)
{
    _type = type;
    _varDrop->setType(type);
}

NoderSPVarArea::NoderSPVarArea(QWidget *parent)
    : NoderSPArea<NoderSPVariableEntry>(parent)
{
    _defValArea = new PzaSpoiler("Default Values", this);
    _defValArea->setVisible(false);

    _addBtn->setText("Add variable");
    _defaultEntryName = DEFAULT_VARIABLE_NAME;
};

NoderSPVariableEntry *NoderSPVarArea::newEntry(void)
{
    NoderSPVariableEntry *entry;

    NoderVar::Type prevType = (_selectedEntry) ? _selectedEntry->type() : DEFAULT_VARIABLE_TYPE;
    NoderVar::Container prevCtn = (_selectedEntry) ? _selectedEntry->container() : DEFAULT_VARIABLE_CONTAINER;
    
    entry = NoderSPArea::newEntry();

    entry->setContainer(prevCtn);
    entry->setType(prevType);

    selectEntry(entry);
    return entry;
}

void NoderSPVarArea::selectEntry(NoderSPVariableEntry *entry)
{
    NoderSPArea::selectEntry(entry);
}

void NoderSPVarArea::removeEntry(NoderSPVariableEntry *entry)
{
    NoderSPArea::removeEntry(entry);
}