#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    public:
        MainWindow();
        ~MainWindow();

    private:
        QWidget _mainWidget;
        QVBoxLayout _mainLayout;
        QColor _backgroundcol;

        void loadTheme(QString filename);
        void setBackgroundCol(QString color);
        QColor backgroundCol() const { return _backgroundcol; };
};

#endif