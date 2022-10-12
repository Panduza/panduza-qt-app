#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class For : public GNodeBranch
{
    public:
        For();
        ~For() = default;

        GNode *branch(void) override;
    
    private:
        PinDecl::Int *_start;
        PinDecl::Int *_end;
        PinDecl::Exec *_scope;
        PinDecl::Exec *_done;
        PinDecl::Int *_index;
};

class ForEachInArray : public GNodeBranch
{
    public:
        ForEachInArray();
        ~ForEachInArray() = default;

        GNode *branch(void) override;

        virtual void onEventConnect(void) override;
        virtual void onEventDisconnect(void) override;
    
    private:
        PinDecl::Array *_array;
        Pin *_elem;
        PinDecl::Exec *_scope;
        PinDecl::Exec *_done;
};