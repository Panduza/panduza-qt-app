#include "NoderSPPin.hpp"

NoderSPPinArea::NoderSPPinArea(QWidget *parent)
    : NoderSPArea<NoderSPPinEntry>(parent)
{
    _addBtn->setText("Add pin");
    _defaultEntryName = DEFAULT_PIN_NAME;
};

NoderSPPinEntry *NoderSPPinArea::newEntry(void)
{
    NoderVar::Container prevCtn;
    NoderVar::Type prevType;
    PinProperty::Direction prevDir;
    NoderSPPinEntry *entry;

    prevCtn = (_selectedEntry) ? _selectedEntry->container() : DEFAULT_PIN_CONTAINER;
    prevType = (_selectedEntry) ? _selectedEntry->type() : DEFAULT_PIN_TYPE;
    prevDir = (_selectedEntry) ? _selectedEntry->direction() : DEFAULT_PIN_DIRECTION;

    entry = NoderSPArea::newEntry();

    connect(entry, &NoderSPPinEntry::directionChanged, this, [&, entry]() {
        directionChanged(entry);
    });

    connect(entry, &NoderSPPinEntry::varChanged, this, [&, entry]() {
        varChanged(entry);
    });

    entry->setDirection(prevDir);
    entry->setContainer(prevCtn);
    entry->setType(prevType);

    selectEntry(entry);
    return entry;
}

void NoderSPPinArea::removeEntry(NoderSPPinEntry *target)
{
    pinRemoved(target);
    NoderSPArea::removeEntry(target);
}

void NoderSPPinArea::selectEntry(NoderSPPinEntry *target)
{
    NoderSPArea::selectEntry(target);
}

NoderSPPinEntry::NoderSPPinEntry(QWidget *parent)
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

void NoderSPPinEntry::setContainer(NoderVar::Container ctn)
{
    _ctn = ctn;
    _varDrop->setContainer(ctn);
}

void NoderSPPinEntry::setType(NoderVar::Type type)
{
    _type = type;
    _varDrop->setType(type);
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
    Noder::Get().forEachPinDirection([&](PinProperty::Direction dir) {
        insertItem(0, Noder::Get().pinDirName(dir));
    });
    connect(this, &PzaComboBox::currentIndexChanged, this, [&](int index) {
        directionChanged(Noder::Get().pinDirFromName(itemText(index)));
    });
    setCurrentIndex(0);
}

void NoderSPDirBox::setDirection(PinProperty::Direction direction)
{
    setCurrentText(Noder::pinDirName(direction));
}