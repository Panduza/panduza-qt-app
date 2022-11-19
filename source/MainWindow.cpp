// Panduza
#include "MainWindow.hpp"
#include "CentralWidget.hpp"

#include "MainStatusBar.hpp"


#include <QTimer>
#include <QFileDialog>


// ============================================================================
// 
MainWindow::MainWindow()
    : QMainWindow(),
    _mainLayout(&_mainWidget)
{


    // qDebug() << styleSheet();
    loadCssStyleFile(":/styles/base");




    resize(800, 600);

    loadTheme("DefaultTheme.json");


    // PanduzaEngine *engine = new PanduzaEngine();

    PzaMenuBar *menuBar();

    // Set a custom status bar
    setStatusBar(new MainStatusBar);

    // Create the central widget
    auto central_widget = new CentralWidget();

    
    
    setCentralWidget(central_widget);
    show();


    // Open data store
    Store::Get().open();




    QTimer::singleShot(0, [this](){

        if( Store::Get().workspace.path().isEmpty() )
        {        
            auto filepath = QFileDialog::getExistingDirectory(this, "Select Workspace");
            if(!filepath.isEmpty())
            {
                qDebug() << ">> " << filepath;
                Store::Get().workspace.setPath(filepath);
            }

            if( Store::Get().workspace.path().isEmpty() )
            {
                QApplication::exit(1);
            }
        }

        
    });



}

void MainWindow::loadCssStyleFile(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file" << filename;
        return;
    }
    
    setStyleSheet(file.readAll());
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
