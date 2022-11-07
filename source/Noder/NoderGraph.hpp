#pragma once

#include <PzaSplitter.hpp>
#include <PzaTabWidget.hpp>
#include "NoderScenario.hpp"
#include "NoderFunction.hpp"
#include "NoderOutputArea.hpp"

class NoderGraph : public PzaSplitter
{
    Q_OBJECT

    friend class NoderFrame;

    public:
        void setActive(NoderFunction *function);
        NoderGraphicsView *getGraphicView(void);

    private:
        NoderGraph(QWidget *parent = nullptr);

        NoderOutputArea Output;
        PzaTabWidget *_tab;

    public slots:
        void newFunction(NoderFunction *scenario);
};