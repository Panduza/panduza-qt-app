#include "NoderSPPin.hpp"

NoderSPPinArea::NoderSPPinArea(QWidget *parent)
    : NoderSPArea<NoderSPPinEntry>(parent)
{
    _addBtn->setText("Add pin");
    _defaultEntryName = DEFAULT_PIN_NAME;
};

NoderSPPinEntry *NoderSPPinArea::newEntry(void)
{
    NoderVarProps prevVar;
    PinProperty::Direction prevDir;
    NoderSPPinEntry *entry;

    if (_selectedEntry) {
        prevVar = _selectedEntry->varProps();
        prevDir = _selectedEntry->direction();
    }
    else {
        DEFAULT_VARIABLE_VAR(prevVar);
        prevDir = DEFAULT_PIN_DIRECTION;
    }

    entry = NoderSPArea::newEntry();

    connect(entry, &NoderSPPinEntry::directionChanged, this, [&, entry]() {
        directionChanged(entry);
    });

    connect(entry, &NoderSPPinEntry::varChanged, this, [&, entry]() {
        pinChanged(entry);
    });

    entry->setDirection(prevDir);
    entry->setVar(prevVar);

    selectEntry(entry);

    return entry;
}

void NoderSPPinArea::removeEntry(NoderSPPinEntry *target)
{
    qDebug() << "LIST DEL" << this << target;
    pinRemoved(target);
    NoderSPArea::removeEntry(target);
}

void NoderSPPinArea::selectEntry(NoderSPPinEntry *target)
{
    NoderSPArea::selectEntry(target);
}

NoderSPPinEntry::NoderSPPinEntry(QWidget *parent)
    : NoderSPEntry<PinVariable>(parent)
{
    _dirBox = new NoderSPDirBox(this);
    _varPicker = new NoderSPVarPicker(this);

    connect(_varPicker, &NoderSPVarPicker::varChanged, this, [&](const NoderVarProps &varProps) {
        _varProps = varProps;
        varChanged();
    });

    connect(_dirBox, &NoderSPDirBox::directionChanged, this, [&](PinProperty::Direction dir) {
        _direction = dir;
        directionChanged();
    });

    _layout->insertWidget(1, _dirBox);
    _layout->insertWidget(2, _varPicker);
}

void NoderSPPinEntry::setVar(const NoderVarProps &varProps)
{
    _varProps = varProps;
    _varPicker->setVar(varProps);
}

void NoderSPPinEntry::setDirection(PinProperty::Direction direction)
{
    _direction = direction;
    _dirBox->setDirection(direction);
    directionChanged();
}

NoderSPDirBox::NoderSPDirBox(QWidget *parent)
    : PzaComboBox(parent)
{
    Noder::Get().ForEachPinDirection([&](PinProperty::Direction dir) {
        insertItem(0, Noder::Get().PinDirName(dir));
    });
    connect(this, &PzaComboBox::currentIndexChanged, this, [&](int index) {
        directionChanged(Noder::Get().PinDirFromName(itemText(index)));
    });
    setCurrentIndex(0);
}

void NoderSPDirBox::setDirection(PinProperty::Direction direction)
{
    setCurrentText(Noder::PinDirName(direction));
}