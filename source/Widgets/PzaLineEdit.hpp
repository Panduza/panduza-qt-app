#pragma once

#include <QLineEdit>
#include <PzaWidget.hpp>

class PzaLineEdit : public QLineEdit
{
    Q_OBJECT

    public:
        PzaLineEdit(const QString &name, QWidget *parent = nullptr);
        PzaLineEdit(QWidget *parent = nullptr);

    protected:
        virtual void focusInEvent(QFocusEvent *e);
        virtual void focusOutEvent(QFocusEvent *e);

    signals:
        void focusIn(void);
        void focusOut(void);
};