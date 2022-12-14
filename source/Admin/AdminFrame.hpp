#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QDockWidget>
#include <QSplitter>
#include <QDebug>
#include <QTextEdit>

#include "NoderView.hpp"
#include "NoderSidePanel.hpp"
#include <PzaMenuBar.hpp>
#include <PzaMenu.hpp>




class TabSystem;
class CfgTreeEditor;
class TabData;


class AdminFrame : public QWidget
{
    public:
    
        AdminFrame();
        
    private:



    QStackedWidget* _content;


    TabSystem* mTabTool;

    CfgTreeEditor* mCfgTreeEditor;


};

