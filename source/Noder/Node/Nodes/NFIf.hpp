#pragma once

#include <GNodeBranch.hpp>
#include <GNodeOp.hpp>

class If : public GNodeBranch
{
    public:
        If();
        ~If() = default;

        GNode *branch(void);
    
    private:
        PinDecl::Bool *_cond;
        PinDecl::Exec *_true;
        PinDecl::Exec *_false;
};