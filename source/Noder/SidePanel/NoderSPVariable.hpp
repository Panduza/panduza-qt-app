#pragma once

#include <QFormLayout>

#include "NoderSidePanel.hpp"
#include "NoderVariable.hpp"
#include "NoderSPVarPicker.hpp"


class NoderSPVariableEntry : public NoderSPEntry<NoderVariable>
{
    Q_OBJECT

    public:
        NoderSPVariableEntry(QWidget *parent = nullptr);

        void remove(void) override {removed();}
        void setVar(const NoderVarProps &varProps);
        const NoderVarProps &varProps() {return _var;} 
        void newVariable(const NoderVarProps &varProps);
        void setGlobal(bool state) {_isGlobal = state;}
        void setFunction(NoderFunction *f);

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        NoderVarProps _var;
        NoderSPVarPicker *_varPicker;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;
        bool _isGlobal;
        NoderFunction *_function;

    signals:
        void varChanged(NoderVariable *var);
        void removed(void);
};

class NoderSPVarArea : public NoderSPArea<NoderSPVariableEntry>
{
    public:
        NoderSPVarArea(QWidget *parent = nullptr);

        NoderSPVariableEntry *newEntry(void);
        void selectEntry(NoderSPVariableEntry *entry);
        void removeEntry(NoderSPVariableEntry *entry);
        void setGlobal(bool state) {_isGlobal = state;}
        void setFunction(NoderFunction *f) {_function = f;}
        NoderSPVariableEntry *findVariableEntry(const QString &name);

        void removeAllEntries(void) override {
            PzaUtils::ForEachDeletedInVector<NoderSPVariableEntry *>(_entryList, [&](NoderSPVariableEntry *entry) {
                removeEntry(entry);
            });
        }

    private:
        bool _isGlobal;
        NoderFunction *_function;
};