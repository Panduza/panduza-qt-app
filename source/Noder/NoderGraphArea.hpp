#pragma once

#include <PzaSplitter.hpp>
#include <PzaTabWidget.hpp>
#include "NoderOutputArea.hpp"
#include "NoderScenario.hpp"
#include "NoderFunction.hpp"
#include "NoderMainEvent.hpp"

class NoderGraphArea : public PzaSplitter
{
    Q_OBJECT

    public:
        NoderGraphArea(QWidget *parent = nullptr);
        ~NoderGraphArea() = default;

    private:
        PzaTabWidget *_tab;
        NoderOutputArea *_outputArea;

    public slots:
        void newScenario(NoderScenario *scenario);
        void newFunction(NoderFunction *scenario);
};