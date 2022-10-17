#pragma once

#include <QLineEdit>

class PzaLineEdit : public QLineEdit
{
    Q_OBJECT

    public:
        PzaLineEdit(const QString &name, QWidget *parent = nullptr);
        PzaLineEdit(QWidget *parent = nullptr);
        ~PzaLineEdit() = default;

    protected:
        virtual void focusInEvent(QFocusEvent *e);
        virtual void focusOutEvent(QFocusEvent *e);

    signals:
        void focusIn(void);
        void focusOut(void);
};