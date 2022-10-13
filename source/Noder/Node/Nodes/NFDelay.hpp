#pragma once

#include <QThread>

#include <GNodeExec.hpp>

class Delay : public GNodeExec
{
    public:
        Delay();
        ~Delay() = default;

        void exec(void) override;
    private:
        PinDecl::Float *_delay;
};