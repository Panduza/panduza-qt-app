#include "PzaLineEdit.hpp"

PzaLineEdit::PzaLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this, &QLineEdit::editingFinished, this, [&](){
        clearFocus();
    });
}

PzaLineEdit::PzaLineEdit(const QString &name, QWidget *parent)
    : PzaLineEdit(parent)
{
    setText(name);    
}

void PzaLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    focusIn();
}

void PzaLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    focusOut();
}