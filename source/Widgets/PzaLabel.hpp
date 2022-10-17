#pragma once

#include <QLabel>

class PzaLabel : public QLabel
{
    public:
        PzaLabel(const QString &name, QWidget *parent = nullptr);
        PzaLabel(QWidget *parent = nullptr);
        ~PzaLabel() = default;
};