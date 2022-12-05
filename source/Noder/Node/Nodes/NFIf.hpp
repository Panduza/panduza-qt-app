#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class If : public GNodeBranch
{
    public:
        If();

        GNode *branch(void) override;
    
    private:
        PinDecl::Ref::Bool *_cond;
        PinDecl::Exec *_true;
        PinDecl::Exec *_false;
};