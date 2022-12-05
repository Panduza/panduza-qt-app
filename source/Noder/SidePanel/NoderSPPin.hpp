#pragma once

#include <PzaComboBox.hpp>
#include <Pin.hpp>
#include "NoderSidePanel.hpp"
#include "NoderSPVarPicker.hpp"

class NoderSPDirBox : public PzaComboBox
{
    Q_OBJECT
    
    public:
        NoderSPDirBox(QWidget *parent = nullptr);
        void setDirection(PinProperty::Direction dir);
    
    signals:
        void directionChanged(PinProperty::Direction dir);
};

class NoderSPPinEntry : public NoderSPEntry<PinVariable>
{
    Q_OBJECT
    
    public:
        NoderSPPinEntry(QWidget *parent = nullptr);

        void setVar(const NoderVarProps &varProps);
        void setDirection(PinProperty::Direction direction);

        const NoderVarProps &varProps(void) {return _varProps;}
        PinProperty::Direction direction(void) const {return _direction;}

        void remove(void) override {removed();}

    private:
        NoderSPVarPicker *_varPicker;
        NoderVarProps _varProps;
        PinProperty::Direction _direction;
        NoderSPDirBox *_dirBox;

    signals:
        void varChanged(void);
        void directionChanged(void);
        void removed(void);
};

class NoderSPPinArea : public NoderSPArea<NoderSPPinEntry>
{
    Q_OBJECT
    
    public:
        NoderSPPinArea(QWidget *parent = nullptr);

        NoderSPPinEntry *newEntry(void) override;
        void removeEntry(NoderSPPinEntry *target) override;
        void selectEntry(NoderSPPinEntry *target) override;

        void removeAllEntries(void) override {
            PzaUtils::ForEachDeletedInVector<NoderSPPinEntry *>(_entryList, [&](NoderSPPinEntry *entry) {
                removeEntry(entry);
            });
        }

    signals:
        void pinRemoved(NoderSPPinEntry *elem);
        void pinChanged(NoderSPPinEntry *elem);
        void directionChanged(NoderSPPinEntry *elem);
};
