#include "NoderSP.hpp"
#include "NoderFrame.hpp"

NoderSP::NoderSP(QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    FunctionArea = new NoderFunctionArea(_main);
    VariableArea = new NoderSPVariableArea(_main);

    PzaSpoiler *functionSpoiler = new PzaSpoiler("Functions", _main);
    PzaSpoiler *variableSpoiler = new PzaSpoiler("Variables", _main);

    functionSpoiler->addWidget(FunctionArea);
    variableSpoiler->addWidget(VariableArea);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFixedWidth(800);

    _layout->addWidget(functionSpoiler);
    _layout->addWidget(variableSpoiler);
    _layout->addStretch(1);

    setStyleSheet("background-color: #252525");

    setWidget(_main);
}

void NoderSP::save(void)
{
    QFile file("/home/agouby/toto.json");

    file.open(QIODevice::ReadWrite | QIODevice::Truncate);

    QJsonDocument doc;
    QJsonObject json;

    json["Functions"] = FunctionArea->save();
    //doc.setObject(FunctionArea->save());

    doc.setObject(json);

    file.write(doc.toJson());
}

template <class N>
NoderSPArea<N>::NoderSPArea(QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QVBoxLayout(this);
    _entryTable = new PzaWidget(this);
    _entryTableLayout = new QVBoxLayout(_entryTable);
    _addBtn = new PzaPushButton(this);

    connect(_addBtn, &PzaPushButton::clicked, this, &NoderSPArea::addEntry);

    _layout->addWidget(_addBtn);
    _layout->addWidget(_entryTable);
}

template <class N>
void NoderSPArea<N>::addEntry(void)
{
    N *entry;
    
    auto getNameInVector = [](N *entry) -> const QString & {
        return entry->name();
    };

    const QString &name = PzaUtils::allocateName<N *>(_entryList, _defaultEntryName, getNameInVector);

    entry = new N(this);
    entry->setName(name);

    connect(entry, &N::removed, this, [&, entry](){
        removeEntry(entry);
    });
    connect(entry, &N::clicked, this, [&, entry]() {
        selectEntry(entry);
    });

    _entryList.push_back(entry);
    _entryTableLayout->addWidget(entry);

    selectEntry(entry);
}

template <class N>
void NoderSPArea<N>::removeEntry(N *target)
{
    if (_selectedEntry == target) {
        N *next = nullptr;
        size_t index = PzaUtils::IndexInVector<N *>(_entryList, _selectedEntry) + 1;

        if (index < _entryList.size())
            next = _entryList.at(index);
        else if (index < 2)
            next = nullptr;
        else if (_entryList.size() > 0)
            next = _entryList.at(index - 2);

        selectEntry(next);
    }

    PzaUtils::DeleteFromVector(_entryList, target);
    target->elem()->deleteLater();
    target->deleteLater();
}

template <class N>
void NoderSPArea<N>::selectEntry(N *target)
{
    if (_selectedEntry)
        _selectedEntry->setSelected(false);
    if (target) {
        target->setSelected(true);
    }
    _selectedEntry = target;
}

NoderFunctionArea::NoderFunctionArea(QWidget *parent)
    : NoderSPArea<NoderFunctionEntry>(parent)
{
    _pinSpoiler = new PzaSpoiler("Pins", this);
    _pinSpoiler->setVisible(false);

    _addBtn->setText("Add function");
    _defaultEntryName = DEFAULT_FUNCTION_NAME;

    _layout->addWidget(_pinSpoiler);
};

QJsonArray NoderFunctionArea::save(void)
{
    QJsonArray array;

    for (auto const &item : _entryList) {
        QJsonObject json;
        NoderFunction *func = item->elem();

        json["name"] = func->name();

        array.append(json);
    }

    return array;
}

void NoderFunctionArea::addEntry(void)
{
    NoderSPArea::addEntry();

    _selectedEntry->createPinArea();
    _pinSpoiler->addWidget(_selectedEntry->pinArea());

    if (_pinSpoiler->isVisible() == false)
        _pinSpoiler->setVisible(true);
}

void NoderFunctionArea::removeEntry(NoderFunctionEntry *target)
{
    _pinSpoiler->removeWidget(target->pinArea());

    target->elem()->dead();

    NoderSPArea::removeEntry(target);

    if (_entryList.size() == 0)
        _pinSpoiler->setVisible(false);
}

void NoderFunctionArea::selectEntry(NoderFunctionEntry *target)
{
    NoderSPArea::selectEntry(target);

    if (target && target->pinArea())
        _pinSpoiler->setCurrentWidget(target->pinArea());
}

NoderSPVarDrop::NoderSPVarDrop(QWidget *parent)
    : PzaWidget(parent)
{
    _typeDrop = new PzaToolButton(this);
    _ctnDrop = new PzaComboBox(this);
    _layout = new QHBoxLayout(this);

    _layout->setContentsMargins(0, 0, 0, 0);

    _ctn = DEFAULT_VARIABLE_CONTAINER;
    _type = DEFAULT_VARIABLE_TYPE;

    _ctnDrop->addItem("Variable");
    _ctnDrop->addItem("Array");
    _ctnDrop->setIconSize(QSize(8, 8));
    connect(_ctnDrop, &PzaComboBox::currentIndexChanged, this, [&](int index) {
        NoderVar::Container ctn = Noder::varContainerFromName(_ctnDrop->itemText(index));
        
        setContainer(ctn);
    });

    configureTree();

    _layout->addWidget(_ctnDrop);
    _layout->addWidget(_typeDrop);
}

void NoderSPVarDrop::setContainer(NoderVar::Container ctn)
{
    _ctn = ctn;
    refreshTypeIco();

    _ctnDrop->blockSignals(true);
    _ctnDrop->setCurrentText(Noder::varContainerName(ctn));
    _ctnDrop->blockSignals(false);
    varChanged(_ctn, _type);
}

void NoderSPVarDrop::setType(NoderVar::Type type)
{
    _type = type;
    refreshContainerIco();
    _typeDrop->setText(Noder::varTypeName(type));
    _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(type, false));
    varChanged(_ctn, _type);
}

void NoderSPVarDrop::configureTree(void)
{
    _typeDrop->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    PzaMenu *menu = new PzaMenu(this);

    QWidgetAction *searchBarAction = new QWidgetAction(this);
    PzaLineEdit *searchBar = new PzaLineEdit(this);
    searchBarAction->setDefaultWidget(searchBar);
    
    QWidgetAction *treeAction = new QWidgetAction(this);
    PzaTreeWidget *tree = new PzaTreeWidget(this);
    treeAction->setDefaultWidget(tree);
    tree->setHeaderHidden(true);

    searchBar->setPlaceholderText("Search");

    _variableCat = addCategory("Variables", tree);
    _interfaceCat = addCategory("Interfaces", tree);
    _enumCat = addCategory("Enums", tree);

    tree->setIconSize(QSize(8, 8));

    _variableCat->setExpanded(true);

    Noder::Get().forEachVarType([&](NoderVar::Type type) {
        if (Noder::Get().varTypeProperties(type).canBeInstance) {
            PzaTreeWidgetItem *item = addItem(Noder::Get().varTypeName(type));
            _variableCat->insertChild(0, item);
            item->setSvgIcon(0, Noder::PlugValue(type, false));
        }
    });

    Noder::Get().forEachEnumName([&](const QString &name) {
        PzaTreeWidgetItem *item = addItem(name, _enumCat);
        item->setSvgIcon(0, Noder::PlugValue(NoderVar::Type::Enum, false));
    });

    connect(searchBar, &PzaLineEdit::textChanged, this, [&](const QString &s){
        QTreeWidgetItemIterator it(tree);

        _variableCat->setExpanded(true);
        _interfaceCat->setExpanded(true);
        _enumCat->setExpanded(true);

        while (*it) {
            PzaTreeWidgetItem *item = static_cast<PzaTreeWidgetItem *>(*it);
            if (item->isCategory() == false) {
                bool match = item->text(0).contains(s, Qt::CaseInsensitive);
                (match) ? item->setHidden(false) : item->setHidden(true);
            }
            it++;
        }
    });

    connect(tree, &QTreeWidget::itemClicked, this, [&, menu](QTreeWidgetItem *from,  int column) {
        (void)column;
        PzaTreeWidgetItem *item = static_cast<PzaTreeWidgetItem *>(from);
        PzaTreeWidgetItem *parent;

        if (item->isCategory() == false) {
            parent =  static_cast<PzaTreeWidgetItem *>(item->parent());
            if (parent->text(0) == "Enums") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Enum, false));
                setType(NoderVar::Type::Enum);
            }
            else if (parent->text(0) == "Interfaces") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Interface, false));
                setType(NoderVar::Type::Interface);
            }
            else {
                NoderVar::Type type = Noder::Get().varTypeFromName(item->text(0));
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(type, false));
                setType(type);
            }
            _typeDrop->setText(item->text(0));
            menu->close();
        }
    });

    menu->addAction(searchBarAction);
    menu->addAction(treeAction);

    _typeDrop->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    _typeDrop->setIconSize(QSize(8, 8));
    _typeDrop->setPopupMode(QToolButton::InstantPopup);
    _typeDrop->setMenu(menu);
}

void NoderSPVarDrop::refreshContainerIco(void)
{
    QPixmap pixmap;

    pixmap.loadFromData(Noder::PlugValue(_type, false));
    _ctnDrop->setItemIcon(0, pixmap);

    pixmap.loadFromData(Noder::PlugArray(_type, false));
    _ctnDrop->setItemIcon(1, pixmap);
}

void NoderSPVarDrop::refreshTypeIco(void)
{
    for (int i = 0; i < _variableCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_variableCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_ctn)(Noder::varTypeFromName(child->text(0)), false));
    }

    for (int i = 0; i < _enumCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_enumCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Enum, false));
    }

    _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(_type, false));
}

PzaTreeWidgetItem *NoderSPVarDrop::addItem(const QString &name, PzaTreeWidgetItem *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);
    
    item->setText(0, name);
    return item;
}

PzaTreeWidgetItem *NoderSPVarDrop::addCategory(const QString &name, PzaTreeWidget *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);

    item->setText(0, name);
    item->setCategory(true);
    return item;
}

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

NoderSPVariableArea::NoderSPVariableArea(QWidget *parent)
    : NoderSPArea<NoderSPVariableEntry>(parent)
{
    _defValArea = new PzaSpoiler("Default Values", this);
    _defValArea->setVisible(false);

    _addBtn->setText("Add variable");
    _defaultEntryName = DEFAULT_VARIABLE_NAME;
};

void NoderSPVariableArea::addEntry(void)
{
    NoderVar::Type prevType = (_selectedEntry) ? _selectedEntry->type() : DEFAULT_VARIABLE_TYPE;
    NoderVar::Container prevCtn = (_selectedEntry) ? _selectedEntry->container() : DEFAULT_VARIABLE_CONTAINER;
    
    NoderSPArea::addEntry();

    _selectedEntry->setContainer(prevCtn);
    _selectedEntry->setType(prevType);
}

void NoderSPVariableArea::selectEntry(NoderSPVariableEntry *entry)
{
    NoderSPArea::selectEntry(entry);
}

void NoderSPVariableArea::removeEntry(NoderSPVariableEntry *entry)
{
    NoderSPArea::removeEntry(entry);
}

NoderPinArea::NoderPinArea(QWidget *parent)
    : NoderSPArea<NoderPinEntry>(parent)
{
    _addBtn->setText("Add pin");
    _defaultEntryName = DEFAULT_PIN_NAME;
};

void NoderPinArea::addEntry(void)
{
    NoderVar::Container prevCtn;
    NoderVar::Type prevType;
    PinProperty::Direction prevDir;

    prevCtn = (_selectedEntry) ? _selectedEntry->container() : DEFAULT_PIN_CONTAINER;
    prevType = (_selectedEntry) ? _selectedEntry->type() : DEFAULT_PIN_TYPE;
    prevDir = (_selectedEntry) ? _selectedEntry->direction() : DEFAULT_PIN_DIRECTION;

    NoderSPArea::addEntry();

    connect(_selectedEntry, &NoderPinEntry::directionChanged, this, [&]() {
        NoderPinEntry *entry = static_cast<NoderPinEntry *>(sender());
        directionChanged(entry);
    });

    connect(_selectedEntry, &NoderPinEntry::varChanged, this, [&]() {
        NoderPinEntry *entry = static_cast<NoderPinEntry *>(sender());
        varChanged(entry);
    });

    _selectedEntry->setDirection(prevDir);
    _selectedEntry->setContainer(prevCtn);
    _selectedEntry->setType(prevType);
}

void NoderPinArea::removeEntry(NoderPinEntry *target)
{
    pinRemoved(target);
    NoderSPArea::removeEntry(target);
}

void NoderPinArea::selectEntry(NoderPinEntry *target)
{
    NoderSPArea::selectEntry(target);
}

template <class N>
NoderSPEntry<N>::NoderSPEntry(QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QHBoxLayout(this);
    _deleteBtn = new PzaPushButton(this);
    _propBtn = new PzaPushButton(this);
    _nameEntry = new PzaNameEntry(this);
    _elem = new N();

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(_nameEntry, &PzaNameEntry::newNameRequested, this, [&](const QString &s){
        bool ok = true;
        auto list = Noder::Get().Frame->SidePanel.VariableArea->entryList();
        for (auto const &item : list) {
            if (item->name() == s) {
                ok = false;
                break;
            }
        }
        (ok) ? setName(s) : setName(name());
    });

    connect(_deleteBtn, &PzaPushButton::clicked, [&](){remove();});

    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(5);

    _propBtn->setFixedSize(20, 20);
    _deleteBtn->setFixedSize(20, 20);

    _layout->addWidget(_nameEntry);
    _layout->addWidget(_propBtn);
    _layout->addWidget(_deleteBtn);
}

template <class N>
void NoderSPEntry<N>::setName(const QString &name)
{
    _name = name;
    _nameEntry->setText(name);
    //_elem->setName(name);
}

template <class N>
void NoderSPEntry<N>::setSelected(bool state)
{
    if (state == true) {
        setStyleSheet("background-color: #1B2426");
    }
    else
        setStyleSheet("background-color: transparent");
}

NoderFunctionEntry::NoderFunctionEntry(QWidget *parent)
    : NoderSPEntry<NoderFunction>(parent)
{
    Noder::Get().Frame->Graph.newFunction(_elem);
}

void NoderFunctionEntry::createPinArea(void)
{
    _pinArea = new NoderPinArea(this);

    connect(_pinArea, &NoderPinArea::varChanged, this, &NoderFunctionEntry::updateVar);
    connect(_pinArea, &NoderPinArea::directionChanged, this, &NoderFunctionEntry::updateDirection);
    connect(_pinArea, &NoderPinArea::pinRemoved, this, &NoderFunctionEntry::removePin);
}

void NoderFunctionEntry::updateVar(NoderPinEntry *entry)
{
    PinValue *newPin;
    const QString &name = entry->name();
    PinProperty::Direction direction = Pin::OppositeDirection(entry->direction());
    NoderVar::Type type = entry->type();
    GNode *node;

    switch (entry->direction()) {
        case PinProperty::Direction::Input:
            node = elem()->startNode();
            break;
        case PinProperty::Direction::Output:
            node = elem()->endNode();
            break;
    }

    if (entry->elem()) {
        int index = node->pinIndex(entry->elem());
        newPin = node->addPinFromType(type, name, direction, index);
        node->replacePin(entry->elem(), newPin);
    }
    else
        newPin = node->addPinFromType(type, name, direction);

    entry->setElem(newPin);
}

void NoderFunctionEntry::updateDirection(NoderPinEntry *entry)
{
    PinValue *newPin;
    PinProperty::Direction newDirection;
    GNode *oldNode;
    GNode *newNode;

    switch (entry->direction()) {
        case PinProperty::Direction::Input:
            newNode = elem()->startNode();
            oldNode = elem()->endNode();
            break;
        case PinProperty::Direction::Output:
            newNode = elem()->endNode();
            oldNode = elem()->startNode();
            break;
    }
    
    if (!entry->elem()) {
        // Means that direction has been set for the first time, but not updated.
        // This happens at initialization.
        return ;
    }
    
    newDirection = Pin::OppositeDirection(entry->direction());
    oldNode->deletePin(entry->elem());
    newPin = newNode->addPinFromType(entry->type(), entry->name(), newDirection);
    entry->setElem(newPin);
}

void NoderFunctionEntry::removePin(NoderPinEntry *entry)
{
    GNode *node;

    switch (entry->direction()) {
        case PinProperty::Direction::Input:
            node = elem()->startNode();
            break;
        case PinProperty::Direction::Output:
            node = elem()->endNode();
            break;
    }
    node->deletePin(entry->elem());
    entry->setElem(nullptr);
}

void NoderFunctionEntry::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;

    mimeData->setData("noder/function", "");
    mimeData->setDataPtr(elem());
    drag->setMimeData(mimeData);
    drag->exec();
}

NoderPinEntry::NoderPinEntry(QWidget *parent)
    : NoderSPEntry<PinValue>(parent)
{
    _dirBox = new NoderSPDirBox(this);
    _varDrop = new NoderSPVarDrop(this);

    connect(_varDrop, &NoderSPVarDrop::varChanged, this, [&](NoderVar::Container ctn, NoderVar::Type type) {
        _ctn = ctn;
        _type = type;
        varChanged();
    });

    connect(_dirBox, &NoderSPDirBox::directionChanged, this, [&](PinProperty::Direction dir) {
        _direction = dir;
        directionChanged();
    });

    elem()->deleteLater();
    setElem(nullptr);

    _layout->insertWidget(1, _dirBox);
    _layout->insertWidget(2, _varDrop);
}

void NoderPinEntry::setContainer(NoderVar::Container ctn)
{
    _ctn = ctn;
    _varDrop->setContainer(ctn);
}

void NoderPinEntry::setType(NoderVar::Type type)
{
    _type = type;
    _varDrop->setType(type);
}

void NoderPinEntry::setDirection(PinProperty::Direction direction)
{
    _direction = direction;
    _dirBox->setDirection(direction);
    directionChanged();
}

NoderSPDirBox::NoderSPDirBox(QWidget *parent)
    : PzaComboBox(parent)
{
    Noder::Get().forEachPinDirection([&](PinProperty::Direction dir) {
        insertItem(0, Noder::Get().pinDirName(dir));
    });
    connect(this, &PzaComboBox::currentIndexChanged, this, [&](int index){
        directionChanged(Noder::Get().pinDirFromName(itemText(index)));
    });
    setCurrentIndex(0);
}

void NoderSPDirBox::setDirection(PinProperty::Direction direction)
{
    setCurrentText(Noder::pinDirName(direction));
}