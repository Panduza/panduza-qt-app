#ifndef NODERFRAME_HPP
#define NODERFRAME_HPP

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QDockWidget>
#include <QSplitter>
#include <QLabel>

#include <NoderView.hpp>
#include <NoderDataModel.hpp>
#include <PanduzaEngine.hpp>
#include <PzaMenuBar.hpp>
#include <PzaMenu.hpp>

class NoderFrame : public QWidget
{
    public:
        NoderFrame(PanduzaEngine *engine);
        ~NoderFrame();

    private:
        PanduzaEngine *_engine;
        QVBoxLayout *_main_layout;
        QVBoxLayout *_sidePanelLayout;

        PzaMenuBar *mainMenuBar(void);
        void sizePanel(void);
    private slots:
        void newInterface(Interface *);
};

#endif