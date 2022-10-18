#include <QApplication>
#include "MainWindow.hpp"


// Panduza
#include <Store/Store.hpp>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Panduza");
    QCoreApplication::setOrganizationDomain("panduza.io");
    QCoreApplication::setApplicationName("Panduza Application");



    MainWindow win;

    win.show();
    
    app.exec();

    return 0;
}