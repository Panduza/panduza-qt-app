#pragma once

#include <GNodeInstance.hpp>
#include <NoderVariable.hpp>

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