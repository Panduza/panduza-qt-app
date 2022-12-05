#pragma once

#include <QDebug>
#include <QFormLayout>

#include <PzaWidget.hpp>

class PzaForm : public PzaWidget
{
    public:
        PzaForm(QWidget *parent = nullptr);

        template<typename T>
        T *addRow(const QString &name = "")
        {
            T *widget = new T(this);

            _layout->addRow(name, widget);
            return widget;
        }

        void addRow(const QString &name, QWidget *widget)
        {
            _layout->addRow(name, widget);
        }

        template<typename T>
        void deleteRow(T *widget)
        {
            _layout->removeRow(widget);
        }

        void deleteRow(int row)
        {
            _layout->removeRow(row);
        }

    protected:
        QFormLayout *_layout;
};