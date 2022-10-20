#pragma once

#include <QCheckBox>
#include <PzaWidget.hpp>

class PzaCheckBox : public QCheckBox
{
    public:
        PzaCheckBox(QWidget *parent = nullptr);
        ~PzaCheckBox() = default;
};