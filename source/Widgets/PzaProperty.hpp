#pragma once

#include <QDebug>
#include <QHBoxLayout>
#include <PzaComboBox.hpp>
#include <PzaLabel.hpp>

class PzaProperty : public PzaWidget
{
    Q_OBJECT

    public:
        PzaProperty(const QString &name, QWidget *parent = nullptr);

    protected:
        QHBoxLayout *_layout;
        PzaLabel *_name;
};

class PzaPropertyComboBox : public PzaProperty
{
    public:
        PzaPropertyComboBox(const QString &name, QWidget *parent);

        PzaComboBox *comboBox(void) {return _prop;}

    private:
        PzaComboBox *_prop;
};