#include <QApplication>
#include "MainWindow.hpp"

#include <QColorDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;

    win.show();
    
    app.exec();

    return 0;
}