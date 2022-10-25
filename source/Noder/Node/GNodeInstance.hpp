#pragma once

#include <GNode.hpp>

class GNodeInstance : public GNode
{
    protected:
        GNodeInstance();
        ~GNodeInstance() = default;
        
        GNode *branch(void) override {return nullptr;}
};