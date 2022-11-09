#pragma once

#include <GNodeInstance.hpp>
#include <GNodeExec.hpp>
#include <NoderSPVariable.hpp>
#include <NoderFunction.hpp>

class Instance : public GNodeInstance
{
    Q_OBJECT

    public:
        Instance();

        void exec() override;
        void setVariable(NoderSPVariable *ref);

    private:
        PinValue *_pin = nullptr;
        NoderSPVariable *_var;
    
    private slots:
        void updatePin(void);
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