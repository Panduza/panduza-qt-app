#pragma once

#include <GNodeInstance.hpp>
#include <GNodeExec.hpp>
#include <NoderVariable.hpp>
#include <NoderFunction.hpp>

class Instance : public GNodeInstance
{
    Q_OBJECT

    public:
        Instance();

        void exec() override;
        void setVariable(NoderVariable *ref);

    private:
        Pin *_pin = nullptr;
        NoderVariable *_var;
    
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