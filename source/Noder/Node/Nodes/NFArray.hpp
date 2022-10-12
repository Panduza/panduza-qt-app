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
        PinProperty::Type _type;
        PinDecl::Array *_result;
        std::vector<Pin *> _list;
};