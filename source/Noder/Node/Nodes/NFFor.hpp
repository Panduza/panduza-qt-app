#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class For : public GNodeBranch
{
    public:
        For();

        GNode *branch(void) override;
    
    private:
        PinDecl::Int *_start;
        PinDecl::Int *_end;
        PinExec *_scope;
        PinExec *_done;
        PinDecl::Int *_index;
};

class ForEachInArray : public GNodeBranch
{
    public:
        ForEachInArray();

        GNode *branch(void) override;

        virtual void onEventConnect(void) override;
        virtual void onEventDisconnect(void) override;
    
    private:
        PinDecl::Array *_array;
        PinValue *_elem;
        PinExec *_scope;
        PinExec *_done;
};