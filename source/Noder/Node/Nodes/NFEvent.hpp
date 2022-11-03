
#pragma once

#include <GNodeEvent.hpp>

class EventScenarioStart : public GNodeEventStart
{
    public:
        EventScenarioStart() : GNodeEventStart("Scenario Start") {}
};

class EventFunctionStart : public GNodeEventStart
{
    public:
        EventFunctionStart() : GNodeEventStart("Function Start")
        {
            setEternal(true);
        }
};

class EventFunctionEnd : public GNodeEventEnd
{
    public:
        EventFunctionEnd() : GNodeEventEnd("Function End")
        {
            setEternal(true);
        }
};

class EventFunctionFinal : public GNodeEvent
{
    public:
        EventFunctionFinal() : GNodeEvent("Function")
        {

        }
};