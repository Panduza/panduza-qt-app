#include "PzaLineEdit.hpp"

PzaLineEdit::PzaLineEdit(const QString &name, QWidget *parent)
    : QLineEdit(name, parent)
{
    setStyleSheet(
        "QLineEdit {"
        "   border: 1px solid #3C3C3C;"
        "   border-radius: 2px;"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "   padding: 3px;"
        "}"
        "QLineEdit::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
    );
    connect(this, &QLineEdit::editingFinished, this, [&](){
        clearFocus();
    });
}

PzaLineEdit::PzaLineEdit(QWidget *parent)
    : PzaLineEdit("", parent)
{

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