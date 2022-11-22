#pragma once

#include <PzaComboBox.hpp>
#include <Pin.hpp>
#include "NoderSidePanel.hpp"
#include "NoderSPVarDrop.hpp"

class NoderSPDirBox : public PzaComboBox
{
    Q_OBJECT
    
    public:
        NoderSPDirBox(QWidget *parent = nullptr);
        void setDirection(PinProperty::Direction dir);
    
    signals:
        void directionChanged(PinProperty::Direction dir);
};

class NoderSPPinEntry : public NoderSPEntry<PinValue>
{
    Q_OBJECT
    
    public:
        NoderSPPinEntry(QWidget *parent = nullptr);

        void setContainer(NoderVar::Container ctn);
        void setType(NoderVar::Type type);
        void setDirection(PinProperty::Direction direction);

        NoderVar::Container container(void) const {return _ctn;}
        NoderVar::Type type(void) const {return _type;}
        PinProperty::Direction direction(void) const {return _direction;}
        
        void remove(void) override {removed();}

    private:
        NoderSPVarDrop *_varDrop;
        NoderVar::Container _ctn;
        NoderVar::Type _type;
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

    signals:
        void pinRemoved(NoderSPPinEntry *elem);
        void varChanged(NoderSPPinEntry *elem);
        void directionChanged(NoderSPPinEntry *elem);
};
