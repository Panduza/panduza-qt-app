#include "NoderConsole.hpp"

NoderConsole::NoderConsole(QWidget *parent)
    : PzaWidget(parent)
{
    _layout = new QVBoxLayout(this);
    _console = new PzaTextEdit(this);
    _inputLine = new PzaLineEdit(this);

    _console->setReadOnly(true);

    _layout->addWidget(_console);
    _layout->addWidget(_inputLine);
}