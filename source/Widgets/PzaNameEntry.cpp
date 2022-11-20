#include "PzaNameEntry.hpp"

PzaNameEntry::PzaNameEntry(QWidget *parent)
    : QStackedWidget(parent)
{
    _label = new PzaLabel(this);
    _lineEdit = new PzaLineEdit(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(_lineEdit, &PzaLineEdit::editingFinished, this, [&](){
        newNameRequested(_lineEdit->text());
    });

    connect(_lineEdit, &PzaLineEdit::focusIn, this, [&]() {
        setCurrentWidget(_lineEdit);
        _lineEdit->selectAll();
    });

    connect(_lineEdit, &PzaLineEdit::focusOut, this, [&]() {
        setCurrentWidget(_label);
    });

    addWidget(_label);
    addWidget(_lineEdit);

    setCurrentWidget(_label);
}

PzaNameEntry::PzaNameEntry(const QString &text, QWidget *parent)
    : PzaNameEntry(parent)
{
    setText(text);
}

void PzaNameEntry::mouseDoubleClickEvent(QMouseEvent *event)
{
    _lineEdit->setFocus(Qt::FocusReason::NoFocusReason);
    setCurrentWidget(_lineEdit);
}

void PzaNameEntry::setText(const QString &text)
{
    _label->setText(text);
    _lineEdit->setText(text);
}