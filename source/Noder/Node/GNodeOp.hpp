#pragma once

#include <GNode.hpp>

class GNodeOp : public GNode
{
    protected:
        GNodeOp(const QString &name);
        ~GNodeOp() = default;

        GNode *branch(void) override {return nullptr;}
};