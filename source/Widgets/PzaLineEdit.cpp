#include "PzaLineEdit.hpp"

PzaLineEdit::PzaLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(
        "QLineEdit {"
        "   border: 1px solid #3C3C3C;"
        "   border-radius: 2px;"
        "   color: #DCDCDC;"
        "   background-color: #191919;"
        "   padding: 3px;"
        "}"
        "QLineEdit::hover {"
        "   color: #EFEFEF;"
        "   background-color: #282828;"
        "}"
    );
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