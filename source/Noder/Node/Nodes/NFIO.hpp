#pragma once

#include <GNodeExec.hpp>

class IoInit : public GNodeExec
{
    public:
        IoInit();

        void exec() override;

    private:
        PinDecl::Ref::Interface *_interface;
        PinDecl::Ref::Enum *_direction;
        PinDecl::Ref::Enum *_value;
};

class IoSetDirection : public GNodeExec
{
    public:
        IoSetDirection();

        void exec() override;

    private:
        
};

class IoSetValue : public GNodeExec
{
    public:
        IoSetValue();

        void exec() override;

    private:
        
};