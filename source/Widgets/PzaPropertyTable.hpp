#pragma once

#include <QDebug>
#include <QGridLayout>
#include <PzaProperty.hpp>
#include <PzaLineEdit.hpp>

class PzaPropertyTable : public PzaWidget
{
    public:
        PzaPropertyTable(QWidget *parent = nullptr);
        ~PzaPropertyTable() = default;

        template<typename T>
        T *addProperty(const QString &name = "")
        {
            int row = _layout->rowCount();
            T *widget = new T();

            if (name != "") {
                PzaLabel *label = new PzaLabel(name, this);
                _layout->addWidget(label, row, 0);
            }
            _layout->addWidget(widget, row, 1);
            return widget;
        }

    protected:
        QGridLayout *_layout;
};