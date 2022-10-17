#pragma once

#include <QDebug>
#include <QHBoxLayout>
#include <PzaComboBox.hpp>
#include <PzaLabel.hpp>

class PzaProperty : public QWidget
{
    Q_OBJECT

    public:
        PzaProperty(const QString &name, QWidget *parent = nullptr);
        ~PzaProperty() = default;

    protected:
        QHBoxLayout *_layout;
        PzaLabel *_name;
};

class PzaPropertyComboBox : public PzaProperty
{
    public:
        PzaPropertyComboBox(const QString &name, QWidget *parent);
        ~PzaPropertyComboBox() = default;

        PzaComboBox *comboBox(void) {return _prop;}

    private:
        PzaComboBox *_prop;
};