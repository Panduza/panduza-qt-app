// Panduza
#include "MainWindow.hpp"
#include <NoderFrame.hpp>

#include "StatusBar.hpp"

MainWindow::MainWindow()
    : QMainWindow(),
    _mainLayout(&_mainWidget)
{
    resize(1600, 800);

    loadTheme("DefaultTheme.json");

    _mainLayout.setContentsMargins(0, 0, 0, 0);
    _mainLayout.addWidget(Noder::Get().Frame);

    setCentralWidget(&_mainWidget);
}

void MainWindow::loadTheme(QString filename)
{
    QFile file(":/styles/" + filename);
    QByteArray byteArray;
    QJsonDocument doc;
    QJsonObject topobj;
    QJsonObject obj;
    QString color;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file" << filename;
        return;
    }

    byteArray = file.readAll();
    doc = QJsonDocument::fromJson(byteArray);

    topobj = doc.object()["MainWindow"].toObject();
    color = topobj["BackgroundCol"].toString();

    setBackgroundCol(color);
}

void MainWindow::setBackgroundCol(QString color)
{
    _backgroundcol = QColor(color);
    setStyleSheet("background-color:" + backgroundCol().name());
}

MainWindow::~MainWindow()
{

}
