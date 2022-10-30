#pragma once

#include <QVBoxLayout>

#include <PzaWidget.hpp>
#include <PzaTextEdit.hpp>
#include <PzaLineEdit.hpp>

class NoderConsole : public PzaWidget
{
    public:
        NoderConsole(QWidget *parent = nullptr);

    private:
        QVBoxLayout *_layout;
        PzaTextEdit *_console;
        PzaLineEdit *_inputLine;
};