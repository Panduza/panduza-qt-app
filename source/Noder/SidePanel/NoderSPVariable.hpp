#pragma once

#include "NoderSidePanel.hpp"
#include "NoderSPVarDrop.hpp"

class NoderSPVariableEntry : public NoderSPEntry<PzaWidget>
{
    Q_OBJECT

    public:
        NoderSPVariableEntry(QWidget *parent = nullptr);

        void remove(void) override {removed();}
        void setContainer(NoderVar::Container ctn);
        NoderVar::Container container(void) const {return _ctn;}
        void setType(NoderVar::Type type);
        NoderVar::Type type() const {return _type;}

    private:
        NoderVar::Container _ctn;
        NoderVar::Type _type;
        NoderSPVarDrop *_varDrop;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;

    signals:
        void removed(void);
        
};

class NoderSPVarArea : public NoderSPArea<NoderSPVariableEntry>
{
    public:
        NoderSPVarArea(QWidget *parent = nullptr);

        NoderSPVariableEntry *newEntry(void);
        void selectEntry(NoderSPVariableEntry *entry);
        void removeEntry(NoderSPVariableEntry *entry);

    private:
        PzaSpoiler *_defValArea = nullptr;
};