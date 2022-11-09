#pragma once

#include <QDebug>
#include <QFormLayout>
#include <PzaLabel.hpp>
#include <PzaLineEdit.hpp>

class PzaPropertyTable : public PzaWidget
{
    public:
        PzaPropertyTable(QWidget *parent = nullptr);

        template<typename T>
        T *addProperty(const QString &name = "")
        {
            T *widget = new T(this);
            PzaLabel *label = new PzaLabel(name, this);

            label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

            _layout->addRow(label, widget);
            return widget;
        }

        template<typename T>
        void deleteProperty(T *widget)
        {
            _layout->removeRow(widget);
        }

    protected:
        QFormLayout *_layout;
};