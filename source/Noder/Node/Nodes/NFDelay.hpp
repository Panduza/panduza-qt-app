#pragma once

#include <QThread>

#include <GNodeExec.hpp>

class Delay : public GNodeExec
{
    public:
        Delay();

        void exec(void) override;
    private:
        PinDecl::Ref::Float *_delay;
};