#include "NoderSidePanel.hpp"

NoderSidePanel::NoderSidePanel(QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    FunctionArea = new NoderFunctionArea(_main);
    VariableArea = new NoderVariableArea(_main);

    PzaSpoiler *functionSpoiler = new PzaSpoiler("Functions", _main);
    PzaSpoiler *variableSpoiler = new PzaSpoiler("Variables", _main);

    functionSpoiler->addWidget(FunctionArea);
    variableSpoiler->addWidget(VariableArea);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFixedWidth(500);

    _layout->addWidget(functionSpoiler);
    _layout->addWidget(variableSpoiler);
    _layout->addStretch(1);

    setStyleSheet("background-color: #252525");

    setWidget(_main);
}

template <class N>
NoderSidePanelArea<N>::NoderSidePanelArea(QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QVBoxLayout(this);
    _entryTable = new PzaWidget(this);
    _entryTableLayout = new QVBoxLayout(_entryTable);
    _addBtn = new PzaPushButton(this);

    connect(_addBtn, &PzaPushButton::clicked, this, &NoderSidePanelArea::addEntry);

    _layout->addWidget(_addBtn);
    _layout->addWidget(_entryTable);
}

template <class N>
void NoderSidePanelArea<N>::addEntry(void)
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
void NoderSidePanelArea<N>::removeEntry(N *target)
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
void NoderSidePanelArea<N>::selectEntry(N *target)
{
    if (_selectedEntry)
        _selectedEntry->setSelected(false);
    if (target) {
        target->setSelected(true);
    }
    _selectedEntry = target;
}

NoderFunctionArea::NoderFunctionArea(QWidget *parent)
    : NoderSidePanelArea<NoderFunctionEntry>(parent)
{
    _pinSpoiler = new PzaSpoiler("Pins", this);
    _pinSpoiler->setVisible(false);

    _addBtn->setText("Add function");
    _defaultEntryName = DEFAULT_FUNCTION_NAME;

    _layout->addWidget(_pinSpoiler);
};

void NoderFunctionArea::addEntry(void)
{
    NoderSidePanelArea::addEntry();

    _selectedEntry->createPinArea();
    _pinSpoiler->addWidget(_selectedEntry->pinArea());

    if (_pinSpoiler->isVisible() == false)
        _pinSpoiler->setVisible(true);
}

void NoderFunctionArea::removeEntry(NoderFunctionEntry *target)
{
    _pinSpoiler->removeWidget(target->pinArea());

    NoderSidePanelArea::removeEntry(target);

    if (_entryList.size() == 0)
        _pinSpoiler->setVisible(false);
}

void NoderFunctionArea::selectEntry(NoderFunctionEntry *target)
{
    NoderSidePanelArea::selectEntry(target);

    if (target && target->pinArea())
        _pinSpoiler->setCurrentWidget(target->pinArea());
}

NoderVariableArea::NoderVariableArea(QWidget *parent)
    : NoderSidePanelArea<NoderVariableEntry>(parent)
{
    _defValArea = new PzaSpoiler("Default Values", this);
    _defValArea->setVisible(false);

    _addBtn->setText("Add variable");
    _defaultEntryName = DEFAULT_VARIABLE_NAME;

    _layout->addWidget(_defValArea);
};

void NoderVariableArea::addEntry(void)
{
    NoderPanel::Type prevType;
    NoderVariable *variable;

    prevType = (_selectedEntry) ? _selectedEntry->elem()->type() : DEFAULT_VARIABLE_TYPE;

    NoderSidePanelArea::addEntry();

    variable = _selectedEntry->elem();
    
    connect(_selectedEntry, &NoderVariableEntry::typeChanged, this, [&, variable]() {
        if (variable->defValTable()) {
            _defValArea->removeWidget(variable->defValTable());
        }
        variable->createDefValTable();
        _defValArea->addWidget(variable->defValTable());
    });
    _selectedEntry->setType(prevType);
    _defValArea->addWidget(variable->defValTable());

    if (_defValArea->isVisible() == false)
        _defValArea->setVisible(true);
}

void NoderVariableArea::removeEntry(NoderVariableEntry *target)
{
    NoderSidePanelArea::removeEntry(target);

    if (_entryList.size() == 0)
        _defValArea->setVisible(false);
}

void NoderVariableArea::selectEntry(NoderVariableEntry *target)
{
    NoderSidePanelArea::selectEntry(target);

    if (target && target->elem()->defValTable())
        _defValArea->setCurrentWidget(target->elem()->defValTable());
}

NoderFunctionPinArea::NoderFunctionPinArea(QWidget *parent)
    : NoderSidePanelArea<NoderPinEntry>(parent)
{
    _addBtn->setText("Add pin");
    _defaultEntryName = DEFAULT_PIN_NAME;
};

void NoderFunctionPinArea::addEntry(void)
{
    PinProperty::Type prevType;
    PinProperty::Direction prevDir;

    prevType = (_selectedEntry) ? _selectedEntry->elem()->type() : DEFAULT_PIN_TYPE;
    prevDir = (_selectedEntry) ? _selectedEntry->elem()->direction() : DEFAULT_PIN_DIRECTION;

    NoderSidePanelArea::addEntry();
       
    connect(_selectedEntry, &NoderPinEntry::pinChanged, this, [&]() {
        NoderPinEntry *entry = static_cast<NoderPinEntry *>(sender());
        pinChanged(entry->elem());
    });

    _selectedEntry->setType(prevType);
    _selectedEntry->setDirection(prevDir);
}

void NoderFunctionPinArea::removeEntry(NoderPinEntry *target)
{
    NoderSidePanelArea::removeEntry(target);
}

void NoderFunctionPinArea::selectEntry(NoderPinEntry *target)
{
    NoderSidePanelArea::selectEntry(target);
}

template <class N>
NoderSidePanelEntry<N>::NoderSidePanelEntry(QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QHBoxLayout(this);
    _deleteBtn = new PzaPushButton(this);
    _propBtn = new PzaPushButton(this);
    _nameLabel = new PzaLabel(this);
    _propTable = new PzaPropertyTable(this);
    _propPopup = new PzaPopup(this);
    _elem = new N();

    _propPopup->addWidget(_propTable);
    _propPopup->setValidator();

    _propName = _propTable->addProperty<PzaLineEdit>("Name");

    connect(_propPopup, &PzaPopup::validated, this, [&]() {
        setName(_propName->text());
    });

    connect(_deleteBtn, &PzaPushButton::clicked, [&](){remove();});

    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(5);

    _propBtn->setFixedSize(20, 20);
    _deleteBtn->setFixedSize(20, 20);

    _propBtn->setMenu(_propPopup);

    _layout->addWidget(_nameLabel);
    _layout->addWidget(_propBtn);
    _layout->addWidget(_deleteBtn);
}

template <class N>
void NoderSidePanelEntry<N>::setName(const QString &name)
{
    _name = name;
    _elem->setName(name);
    _nameLabel->setText(name);
    _propName->setText(name);
}

template <class N>
void NoderSidePanelEntry<N>::setSelected(bool state)
{
    if (state == true) {
        setStyleSheet("background-color: #1B2426");
    }
    else
        setStyleSheet("background-color: transparent");
}

NoderVariableEntry::NoderVariableEntry(QWidget *parent)
    : NoderSidePanelEntry<NoderVariable>(parent)
{
    _colorFrame = new QFrame(this);
    _typeLabel = new PzaLabel(this);

    _colorFrame->setFixedSize(10, 10);
    _colorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _propPopup->addTitle("Edit variable");

    _propType = _propTable->addProperty<PzaComboBox>("Type");
    Noder::Get().forEachVarType([&](NoderPanel::Type type) {
        _propType->insertItem(0, Noder::Get().varTypeName(type));
    });
    connect(_propType, &PzaComboBox::currentIndexChanged, this, [&](int index){
        setType(Noder::Get().varTypeFromName(_propType->itemText(index)));
    });

    _layout->insertWidget(1, _colorFrame);
    _layout->insertWidget(2, _typeLabel);
}

void NoderVariableEntry::setType(NoderPanel::Type type)
{
    int index = _propType->findText(Noder::Get().varTypeName(type));
    _propType->setCurrentIndex(index);

    _typeLabel->setText(Noder::Get().varTypeName(type));
    _colorFrame->setStyleSheet(
            "background-color: " + Noder::Get().varColor(type).name() + ";"
            "border-radius: 5px;"
    );
    _elem->setType(type);
    typeChanged();
}

void NoderVariableEntry::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;

    mimeData->setData("noder/variable", "");
    mimeData->setDataPtr(elem());
    drag->setMimeData(mimeData);
    drag->setPixmap(_propName->grab());
    drag->exec();
}

NoderFunctionEntry::NoderFunctionEntry(QWidget *parent)
    : NoderSidePanelEntry<NoderFunction>(parent)
{
    _propPopup->addTitle("Edit function");
    NoderFrame::Get()->Graph->newFunction(_elem);
}

void NoderFunctionEntry::createPinArea(void)
{
    _pinArea = new NoderFunctionPinArea(this);

    connect(_pinArea, &NoderFunctionPinArea::pinChanged, this, &NoderFunctionEntry::updatePin);
}

void NoderFunctionEntry::updatePin(NoderPin *pinEntry)
{
    Pin *pin;
    int index = -1;

    if (pinEntry->direction() == PinProperty::Direction::Input) {
        if (pinEntry->pin()) {
            index = elem()->startNode()->pinIndex(pinEntry->pin());
            // index
            // deletePin

        }
        pin = elem()->startNode()->addPinFromType(pinEntry->type(), pinEntry->name(), Pin::OppositeDirection(pinEntry->direction()), index);
        if (pinEntry->pin())
            elem()->startNode()->replacePin(pinEntry->pin(), pin);
        
        pinEntry->setPin(pin);
    }
}

void NoderFunctionEntry::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;

    mimeData->setData("noder/function", "");
    mimeData->setDataPtr(elem());
    drag->setMimeData(mimeData);
    drag->setPixmap(_propName->grab());
    drag->exec();
}

NoderPinEntry::NoderPinEntry(QWidget *parent)
    : NoderSidePanelEntry<NoderPin>(parent)
{
    _propPopup->addTitle("Edit pin");

    _colorFrame = new QFrame(this);
    _typeLabel = new PzaLabel(this);
    _directionLabel = new PzaLabel(this);

    _colorFrame->setFixedSize(10, 10);
    _colorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _propType = _propTable->addProperty<PzaComboBox>("Type");
    Noder::Get().forEachPinType([&](PinProperty::Type type) {
        _propType->insertItem(0, Noder::Get().pinTypeToStr(type));
    });
    connect(_propType, &PzaComboBox::currentIndexChanged, this, [&](int index){
        setType(Noder::Get().pinTypeFromName(_propType->itemText(index)));
    });

    _propDirection = _propTable->addProperty<PzaComboBox>("Direction");
    Noder::Get().forEachPinDirection([&](PinProperty::Direction direction) {
        _propDirection->insertItem(0, Noder::Get().pinDirToStr(direction));
    });
    connect(_propDirection, &PzaComboBox::currentIndexChanged, this, [&](int index){
        setDirection(Noder::Get().pinDirectionFromName(_propDirection->itemText(index)));
    });

    _layout->insertWidget(1, _directionLabel);
    _layout->insertWidget(2, _colorFrame);
    _layout->insertWidget(3, _typeLabel);
}

void NoderPinEntry::setName(const QString &name)
{
    NoderSidePanelEntry::setName(name);

    if (elem()->pin())
        elem()->pin()->setName(name);
}

void NoderPinEntry::setType(PinProperty::Type type)
{
     int index = _propType->findText(Noder::Get().pinTypeToStr(type));
    _propType->setCurrentIndex(index);

    _typeLabel->setText(Noder::Get().pinTypeToStr(type));
    _colorFrame->setStyleSheet(
            "background-color: " + Noder::Get().plugColor(type).name() + ";"
            "border-radius: 5px;"
    );
    _elem->setType(type);
    pinChanged();
}

void NoderPinEntry::setDirection(PinProperty::Direction direction)
{
     int index = _propDirection->findText(Noder::Get().pinDirToStr(direction));
    _propDirection->setCurrentIndex(index);

    _directionLabel->setText(Noder::Get().pinDirToStr(direction));
    _elem->setDirection(direction);
    pinChanged();
}