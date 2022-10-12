#pragma once

#include <GNode.hpp>

class GNodeInstance : public GNode
{
    protected:
        GNodeInstance(const QString &name);
        ~GNodeInstance() = default;
};