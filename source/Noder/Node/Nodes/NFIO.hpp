#pragma once

#include <GNodeExec.hpp>

class IoInit : public GNodeExec
{
    public:
        IoInit();
        ~IoInit() = default;

        void exec() override;

    private:
        PinDecl::Interface *_interface;
        PinDecl::Enum *_direction;
        PinDecl::Enum *_value;
};

class IoSetDirection : public GNodeExec
{
    public:
        IoSetDirection();
        ~IoSetDirection() = default;

        void exec() override;

    private:
        
};

class IoSetValue : public GNodeExec
{
    public:
        IoSetValue();
        ~IoSetValue() = default;

        void exec() override;

    private:
        
};