#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QDockWidget>
#include <QSplitter>
#include "NoderMenuBar.hpp"
#include "NoderSidePanel.hpp"
#include "NoderGraph.hpp"
#include "Noder.hpp"

class NoderFrame : public PzaWidget
{
    friend class Noder;
    
    public:
        NoderSidePanel SidePanel;
        NoderGraph Graph;
        NoderMenuBar MenuBar;

    private:
        NoderFrame();

        QVBoxLayout *_layout;
};