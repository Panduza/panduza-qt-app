#pragma once

#include <PzaSplitter.hpp>
#include <PzaTabWidget.hpp>
#include "NoderOutputArea.hpp"
#include "NoderScenario.hpp"
#include "NoderFunction.hpp"

class NoderGraph : public PzaSplitter
{
    Q_OBJECT

    public:
        NoderGraph(QWidget *parent = nullptr);

        void setActive(NoderFunction *function);
        NoderGraphicsView *getGraphicView(void);

    private:
        PzaTabWidget *_tab;
        NoderOutputArea *_outputArea;

    public slots:
        void newFunction(NoderFunction *scenario);
};