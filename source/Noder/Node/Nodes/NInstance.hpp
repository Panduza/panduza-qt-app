#pragma once

#include <GNodeInstance.hpp>
#include <NoderVariable.hpp>

class Instance : public GNodeInstance
{
    public:
        Instance(NoderVariable *var);
        ~Instance() = default;

        void exec() override;
        void setVariable(NoderVariable *ref);

    private:
        Pin *_pin = nullptr;
        NoderVariable *_ref;
};