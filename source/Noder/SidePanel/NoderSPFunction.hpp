#pragma once

#include "NoderSidePanel.hpp"
#include "NoderSPPin.hpp"
#include "NoderSPVariable.hpp"
#include "NoderFunction.hpp"

class NoderSPFunctionEntry : public NoderSPEntry<NoderFunction>
{
    Q_OBJECT
    
    public:
        NoderSPFunctionEntry(QWidget *parent = nullptr);
        
        void remove(void) override {removed();}
        NoderSPPinArea *pinArea(void) const {return _pinArea;}
        void setPinArea(NoderSPPinArea *pinArea) {_pinArea = pinArea;}
        NoderSPVarArea *varArea(void) const {return _varArea;}
        void setVarArea(NoderSPVarArea *varArea) {_varArea = varArea;}
        void updateVar(NoderSPPinEntry *pin);
        void updateDirection(NoderSPPinEntry *pin);
        void removePin(NoderSPPinEntry *pin);

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        NoderSPPinArea *_pinArea = nullptr;
        NoderSPVarArea *_varArea = nullptr;

    signals:
        void removed(void);
};

class NoderSPFunctionArea : public NoderSPArea<NoderSPFunctionEntry>
{
    public:
        NoderSPFunctionArea(QWidget *parent = nullptr);

        QJsonArray save(void);

        NoderSPFunctionEntry *newEntry(void) override;
        void removeEntry(NoderSPFunctionEntry *target) override;
        void selectEntry(NoderSPFunctionEntry *target) override;

    private:
        PzaSpoiler *_pinSpoiler;
        PzaSpoiler *_varSpoiler;
};