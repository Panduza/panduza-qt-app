#include "NoderSPFunction.hpp"
#include "NoderFunction.hpp"
#include "NoderFrame.hpp"

#include "NoderSidePanel.hpp"

NoderSPFunctionArea::NoderSPFunctionArea(QWidget *parent)
    : NoderSPArea<NoderSPFunctionEntry>(parent)
{
    _pinSpoiler = new PzaSpoiler("Pins", this);
    _varSpoiler = new PzaSpoiler("Variables", this);

    _pinSpoiler->setVisible(false);
    _varSpoiler->setVisible(false);

    _addBtn->setText("Add function");
    _defaultEntryName = DEFAULT_FUNCTION_NAME;

    _layout->addWidget(_pinSpoiler);
    _layout->addWidget(_varSpoiler);
};

QJsonArray NoderSPFunctionArea::save(void)
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

NoderSPFunctionEntry *NoderSPFunctionArea::newEntry(void)
{
    NoderSPFunctionEntry *entry;
    
    entry = NoderSPArea::newEntry();

    _pinSpoiler->addWidget(entry->pinArea());
    _varSpoiler->addWidget(entry->varArea());

    if (_pinSpoiler->isVisible() == false) {
        _pinSpoiler->setVisible(true);
        _varSpoiler->setVisible(true);
    }

    selectEntry(entry);
    return entry;
}

void NoderSPFunctionArea::removeEntry(NoderSPFunctionEntry *target)
{
    _pinSpoiler->removeWidget(target->pinArea());
    _varSpoiler->removeWidget(target->varArea());

    target->elem()->dead();

    NoderSPArea::removeEntry(target);

    if (_entryList.size() == 0) {
        _pinSpoiler->setVisible(false);
        _varSpoiler->setVisible(false);
    }
}

void NoderSPFunctionArea::selectEntry(NoderSPFunctionEntry *target)
{
    NoderSPArea::selectEntry(target);

    if (target && target->pinArea())
        _pinSpoiler->setCurrentWidget(target->pinArea());

    if (target && target->varArea())
        _varSpoiler->setCurrentWidget(target->varArea());
}

NoderSPFunctionEntry::NoderSPFunctionEntry(QWidget *parent)
    : NoderSPEntry<NoderFunction>(parent)
{
    Noder::Get().Frame->Graph.newFunction(_elem);

    _pinArea = new NoderSPPinArea(this);
    _varArea = new NoderSPVarArea(this);

    connect(_pinArea, &NoderSPPinArea::varChanged, this, &NoderSPFunctionEntry::updateVar);
    connect(_pinArea, &NoderSPPinArea::directionChanged, this, &NoderSPFunctionEntry::updateDirection);
    connect(_pinArea, &NoderSPPinArea::pinRemoved, this, &NoderSPFunctionEntry::removePin);
}

void NoderSPFunctionEntry::updateVar(NoderSPPinEntry *entry)
{
    PinValue *newPin;
    const QString &name = entry->name();
    PinProperty::Direction direction = Pin::OppositeDirection(entry->direction());
    NoderVar::Type type = entry->type();
    GNode *node = nullptr;

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

void NoderSPFunctionEntry::updateDirection(NoderSPPinEntry *entry)
{
    PinProperty::Direction newDirection;
    PinValue *newPin = nullptr;
    GNode *oldNode = nullptr;
    GNode *newNode = nullptr;

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

void NoderSPFunctionEntry::removePin(NoderSPPinEntry *entry)
{
    GNode *node = nullptr;

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

void NoderSPFunctionEntry::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QDrag *drag = new QDrag(this);
    PzaMimeData *mimeData = new PzaMimeData;

    mimeData->setData("noder/function", "");
    mimeData->setDataPtr(elem());
    drag->setMimeData(mimeData);
    drag->exec();
}