#pragma once

#include <GNodeInstance.hpp>
#include <GNodeExec.hpp>
#include <NoderSPVariable.hpp>
#include <NoderFunction.hpp>

class VariableInstance : public GNodeInstance
{
    Q_OBJECT

    public:
        VariableInstance();

        void exec() override;
        void setVariableEntry(NoderSPVariableEntry *ref);

    private:
        PinVariable *_pin = nullptr;
        NoderSPVariableEntry *_varEntry = nullptr;
        NoderVariable *_var = nullptr;
    
    private slots:
        void newVariable(NoderVariable *var);
};

class FuncInstance : public GNodeExec
{
    Q_OBJECT

    public:
        FuncInstance();

        void exec() override {}
        void setFunction(NoderFunction *func);

    private:
        NoderFunction *_function;

    private slots:
        void updateNode(void);
};