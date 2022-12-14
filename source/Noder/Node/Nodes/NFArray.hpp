#pragma once

#include <GNodeOp.hpp>

class Array : public GNodeOp
{
    public:
        Array();
        ~Array();

        void exec(void) override;

        virtual void onEventConnect(void) override;
        virtual void onEventDisconnect(void) override;

    private:
        NoderVarProps _var;
        PinArray *_result;
        std::vector<PinRef *> _list;
};