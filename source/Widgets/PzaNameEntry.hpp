#pragma once

#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>
#include <QStackedWidget>

class PzaNameEntry : public QStackedWidget
{
    Q_OBJECT

    public:
        PzaNameEntry(const QString &text, QWidget *parent = nullptr);
        PzaNameEntry(QWidget *parent = nullptr);

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void setText(const QString &text);
        const QString &text(void) {return _text;}

    private:
        QString _text;
        PzaLabel *_label;
        PzaLineEdit *_lineEdit;
    
    signals:
        void newNameRequested(const QString &text);
};