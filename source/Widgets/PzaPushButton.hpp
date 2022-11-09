#pragma once

#include <QPushButton>

class PzaPushButton : public QPushButton
{
    public:
        PzaPushButton(QWidget *parent = nullptr);
        PzaPushButton(const QString &name, QWidget *parent = nullptr);
};