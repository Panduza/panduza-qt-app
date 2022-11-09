#pragma once

#include <QToolButton>

class PzaToolButton : public QToolButton
{
    public:
        PzaToolButton(QWidget *parent = nullptr);
        PzaToolButton(const QString &name, QWidget *parent = nullptr);

        void setIcon(const QByteArray &data);
};