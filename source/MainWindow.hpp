#pragma once

#include <iostream>
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

        void loadCssStyleFile(const QString& filename);
};