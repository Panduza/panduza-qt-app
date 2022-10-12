#pragma once

#include <GNodeExec.hpp>
#include <GNodeOp.hpp>

class Print : public GNodeExec
{
    public:
        Print();
        ~Print() = default;

        void exec() override;

    private:
        PinDecl::String *_string;
        
};

class Cat : public GNodeOp
{
    public:
        Cat();
        ~Cat() = default;

        void exec() override;

    private:
        std::vector<Pin *> _list;
        PinDecl::String *_spacer;
        PinDecl::String *_result;
};