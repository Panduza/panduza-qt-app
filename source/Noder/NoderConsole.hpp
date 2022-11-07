#pragma once

#include <QVBoxLayout>

#include <PzaWidget.hpp>
#include <PzaTextEdit.hpp>
#include <PzaLineEdit.hpp>

class NoderConsole : public PzaWidget
{
    friend class NoderOutputArea;

    private:
        NoderConsole(QWidget *parent = nullptr);

        QVBoxLayout *_layout;
        PzaTextEdit *_console;
        PzaLineEdit *_inputLine;
};