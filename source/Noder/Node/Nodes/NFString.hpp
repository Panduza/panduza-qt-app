#pragma once

#include <GNodeExec.hpp>
#include <GNodeOp.hpp>

class Print : public GNodeExec
{
    public:
        Print();

        void exec() override;

    private:
        std::vector<PinRef *> _list;
        
};

class Cat : public GNodeOp
{
    public:
        Cat();

        void exec() override;

    private:
        std::vector<PinRef *> _list;
        PinDecl::Ref::String *_spacer;
        PinDecl::Ref::String *_result;
};