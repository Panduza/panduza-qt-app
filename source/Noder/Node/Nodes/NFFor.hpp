#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class For : public GNodeBranch
{
    public:
        For();

        GNode *branch(void) override;
    
    private:
        PinDecl::Ref::Int *_start;
        PinDecl::Ref::Int *_end;
        PinDecl::Exec *_scope;
        PinDecl::Exec *_done;
        PinDecl::Ref::Int *_index;
};

class ForEachInArray : public GNodeBranch
{
    public:
        ForEachInArray();

        GNode *branch(void) override;

        virtual void onEventConnect(void) override;
        virtual void onEventDisconnect(void) override;
    
    private:
        //PinDecl::Ref::Array *_array;
        PinRef *_elem;
        PinDecl::Exec *_scope;
        PinDecl::Exec *_done;
};