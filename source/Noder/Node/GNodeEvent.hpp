#pragma once

#include <GNodeExec.hpp>

class GNodeEventStart : public GNodeExec
{
    protected:
        GNodeEventStart(const QString &name);
};

class GNodeEventEnd: public GNodeExec
{
    protected:
        GNodeEventEnd(const QString &name);
};

class GNodeEvent: public GNodeExec
{
    protected:
        GNodeEvent(const QString &name);
};