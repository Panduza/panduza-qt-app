#pragma once

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QWidget>

class Style : public QObject
{
    Q_OBJECT

    public:
        Style();

        void loadCssFromFile(const QString &filename);
        void bindStyleSheet(QWidget *w)
        {
            connect(this, &Style::updated, this, [&, w]() {
                w->setStyleSheet(stylesheet);
            });
            w->setStyleSheet(stylesheet);
        }

    private:
        QString stylesheet;

    signals:
        void updated(void);
};