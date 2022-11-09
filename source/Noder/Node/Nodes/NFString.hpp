#pragma once

#include <GNodeExec.hpp>
#include <GNodeOp.hpp>

class Print : public GNodeExec
{
    public:
        Print();

        void exec() override;

    private:
        std::vector<PinValue *> _list;
        
};

class Cat : public GNodeOp
{
    public:
        Cat();

        void exec() override;

    private:
        std::vector<PinValue *> _list;
        PinDecl::String *_spacer;
        PinDecl::String *_result;
};