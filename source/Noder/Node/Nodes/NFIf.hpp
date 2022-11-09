#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class If : public GNodeBranch
{
    public:
        If();

        GNode *branch(void) override;
    
    private:
        PinDecl::Bool *_cond;
        PinExec *_true;
        PinExec *_false;
};