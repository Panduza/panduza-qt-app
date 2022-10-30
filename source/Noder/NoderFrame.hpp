#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QDockWidget>
#include <QSplitter>

#include "Noder.hpp"

class NoderSidePanel;
class NoderGraph;
class NoderMenuBar;

class NoderFrame : public PzaWidget
{
    public:

        NoderFrame(NoderFrame &other) = delete;
        void operator=(const NoderFrame &) = delete;

        static NoderFrame *Get(void)
        {
            static NoderFrame *frame;

            if (frame == nullptr)
                frame = new NoderFrame();
            return frame;
        }

        NoderSidePanel *SidePanel = nullptr;
        NoderGraph *Graph = nullptr;
        NoderMenuBar *MenuBar = nullptr;

    private:
        NoderFrame();

        QVBoxLayout *_layout;
};