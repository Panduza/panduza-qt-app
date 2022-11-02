
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
        EventFunctionStart() : GNodeEventStart("Function Start") {}
};