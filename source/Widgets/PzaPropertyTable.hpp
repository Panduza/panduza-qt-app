#pragma once

#include <QDebug>
#include <QGridLayout>
#include <PzaProperty.hpp>
#include <PzaLabel.hpp>
#include <PzaLineEdit.hpp>

class PzaPropertyTable : public PzaWidget
{
    public:
        PzaPropertyTable(QWidget *parent = nullptr);

        template<typename T>
        T *addProperty(const QString &name = "")
        {
            int row = _grid->rowCount();
            T *widget = new T();

            if (name != "") {
                PzaLabel *label = new PzaLabel(name, this);
                _grid->addWidget(label, row, 0);
            }
            _grid->addWidget(widget, row, 1);
            return widget;
        }

    protected:
        QGridLayout *_grid;
};