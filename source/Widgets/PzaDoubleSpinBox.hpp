#pragma once

#include <QDoubleSpinBox>
#include <PzaWidget.hpp>

class PzaDoubleSpinBox : public QDoubleSpinBox
{
    public:
        PzaDoubleSpinBox(QWidget *parent = nullptr);
        ~PzaDoubleSpinBox() = default;
};