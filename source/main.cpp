// Qt
#include <QDebug>
#include <QApplication>

// Panduza
#include "MainWindow.hpp"
#include <Store/Store.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Panduza");
    QCoreApplication::setOrganizationDomain("panduza.io");
    QCoreApplication::setApplicationName("Panduza Application");


    // DOES NOT WORK HERE
    // QFile file(":/styles/base");
    // if (!file.open(QIODevice::ReadOnly)) {
    //     qWarning() << "Could not open file !!!";
    //     return 1;
    // }
    // auto content = QLatin1String((file.readAll()));
    // qDebug() << content;
    // app.setStyleSheet(content);


    MainWindow win;

    win.show();
    
    app.exec();

    return 0;
}