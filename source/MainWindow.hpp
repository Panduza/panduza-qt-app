#pragma once

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
#include <PzaWidget.hpp>

class MainWindow : public QMainWindow
{
    public:
        MainWindow();
        ~MainWindow();

    private:
        PzaWidget _mainWidget;
        QVBoxLayout _mainLayout;
        QColor _backgroundcol;

        void loadTheme(QString filename);
        void setBackgroundCol(QString color);
        QColor backgroundCol() const { return _backgroundcol; };
};