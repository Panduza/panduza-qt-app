#pragma once

#include <GNode.hpp>

class GNodeInstance : public GNode
{
    protected:
        GNodeInstance(const QString &name);
        ~GNodeInstance() = default;
        
        GNode *branch(void) override {return nullptr;}
};