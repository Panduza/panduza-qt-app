#include "NoderSPVariable.hpp"
#include "NoderFrame.hpp"


NoderSPVariableEntry::NoderSPVariableEntry(QWidget *parent)
    : NoderSPEntry<NoderVariable>(parent)
{
    _varPicker = new NoderSPVarPicker(this);

    connect(_varPicker, &NoderSPVarPicker::varChanged, this, [&](const NoderVarProps &varProps) {
        newVariable(varProps);
    });

    _layout->insertWidget(1, _varPicker);
}

void NoderSPVariableEntry::setFunction(NoderFunction *f)
{
    _function = f;
}

void NoderSPVariableEntry::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;
    QJsonDocument doc;
    QJsonObject obj;

    obj["Object Type"] = "Variable";
    obj["Name"] = _name;
    obj["Global"] = _isGlobal;
    if (_isGlobal == false)
        obj["Function"] = _function->name();

    doc.setObject(obj);

    mimeData->setData("application/json", doc.toJson());
    drag->setMimeData(mimeData);
    drag->exec();
}

void NoderSPVariableEntry::newVariable(const NoderVarProps &varProps)
{
    NoderVariable *newVar = nullptr;

    if (_elem) {
        if (_isGlobal == false) {
            _function->deleteVariable(_elem);
        }
        Noder::Get().Frame->SidePanel.DefValArea->removeWidget(_elem->defVal());
        _elem->deleteLater();
    }

    switch (varProps.container) {
        case NoderVarProps::Container::Array:
        {
            switch (varProps.type) {
                case NoderVarProps::Type::Bool:
                    newVar = new NoderArrayBool(varProps, this);
                    break;
                case NoderVarProps::Type::Int:
                    newVar = new NoderArrayInt(varProps, this);
                    break;
                case NoderVarProps::Type::Float:
                    newVar = new NoderArrayFloat(varProps, this);
                    break;
                case NoderVarProps::Type::String:
                    newVar = new NoderArrayString(varProps, this);
                    break;
                case NoderVarProps::Type::Enum:
                    newVar = new NoderArrayEnum(varProps, this);
                    break;
                case NoderVarProps::Type::Interface:
                    newVar = new NoderArrayInterface(varProps, this);
                    break;
                default:
                    break;
            }
            break;
        }
        case NoderVarProps::Container::Reference:
        {
            switch (varProps.type) {
                case NoderVarProps::Type::Bool:
                    newVar = new NoderRefBool(this);
                    break;
                case NoderVarProps::Type::Int:
                    newVar = new NoderRefInt(this);
                    break;
                case NoderVarProps::Type::Float:
                    newVar = new NoderRefFloat(this);
                    break;
                case NoderVarProps::Type::String:
                    newVar = new NoderRefString(this);
                    break;
                case NoderVarProps::Type::Enum:
                    newVar = new NoderRefEnum(varProps.subType, this);
                    break;
                case NoderVarProps::Type::Interface:
                    newVar = new NoderRefInterface(varProps.subType, this);
                    break;
                default:
                    break;
            }
            break;
        }
    }

    _var = varProps;
    _elem = newVar;

    if (newVar) {
        Noder::Get().Frame->SidePanel.DefValArea->addWidget(newVar->defVal());
        newVar->setGlobal(_isGlobal);
        newVar->setName(name());
        newVar->setFunction(_function);
        varChanged(newVar);
    }
}

void NoderSPVariableEntry::setVar(const NoderVarProps &varProps)
{
    _var = varProps;
    _varPicker->setVar(varProps);
}

NoderSPVarArea::NoderSPVarArea(QWidget *parent)
    : NoderSPArea<NoderSPVariableEntry>(parent),
    _isGlobal(false),
    _function(nullptr)
{
    _addBtn->setText("Add variable");
    _defaultEntryName = DEFAULT_VARIABLE_NAME;
};

NoderSPVariableEntry *NoderSPVarArea::findVariableEntry(const QString &name)
{
    for (auto const &item : _entryList) {
        if (item->name() == name)
            return item;
    }
    return nullptr;
}

NoderSPVariableEntry *NoderSPVarArea::newEntry(void)
{
    NoderSPVariableEntry *entry;
    NoderVarProps prevVar;
    
    if (_selectedEntry)
        prevVar = _selectedEntry->varProps();
    else
        DEFAULT_VARIABLE_VAR(prevVar);

    entry = NoderSPArea::newEntry();

    entry->setGlobal(_isGlobal);
    entry->setFunction(_function);
    entry->setVar(prevVar);
    entry->newVariable(entry->varProps());

    selectEntry(entry);

    return entry;
}

void NoderSPVarArea::selectEntry(NoderSPVariableEntry *entry)
{
    if (entry && entry->elem()) {
        Noder::Get().Frame->SidePanel.DefValArea->setCurrentWidget(entry->elem()->defVal());
    }
    
    NoderSPArea::selectEntry(entry);
}

void NoderSPVarArea::removeEntry(NoderSPVariableEntry *entry)
{
    if (_isGlobal == false) {
        _function->deleteVariable(entry->elem());
    }
    
    Noder::Get().Frame->SidePanel.DefValArea->removeWidget(entry->elem()->defVal());

    entry->elem()->deleteLater();

    NoderSPArea::removeEntry(entry);
}