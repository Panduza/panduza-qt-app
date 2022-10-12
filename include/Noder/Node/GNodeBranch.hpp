#pragma once

#include <GNodeExec.hpp>

class GNodeBranch : public GNodeExec
{
    protected:
        GNodeBranch(const QString &name);
        ~GNodeBranch() = default;
};