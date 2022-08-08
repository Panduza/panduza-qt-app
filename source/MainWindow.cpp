#include <MainWindow.hpp>
#include <NoderFrame.hpp>
#include <PanduzaEngine.hpp>

MainWindow::MainWindow()
    : QMainWindow(),
    _mainLayout(&_mainWidget)
{
    resize(800, 600);

    loadTheme("DefaultTheme.json");

    PanduzaEngine *engine = new PanduzaEngine();

    PzaMenuBar *menuBar();
    NoderFrame *frame = new NoderFrame(engine);

    _mainLayout.setContentsMargins(0, 0, 0, 0);
    _mainLayout.addWidget(frame);
    
    setCentralWidget(&_mainWidget);
    show();
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