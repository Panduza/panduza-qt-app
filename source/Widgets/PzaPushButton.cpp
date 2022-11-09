#include "PzaPushButton.hpp"

PzaPushButton::PzaPushButton(QWidget *parent)
    : QPushButton(parent)
{
    setAutoDefault(true);
}

PzaPushButton::PzaPushButton(const QString &name, QWidget *parent)
    : PzaPushButton(parent)
{
    setText(name);
}