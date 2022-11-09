#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDrag>

#include "Noder.hpp"
#include <PzaWidget.hpp>
#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>
#include <PzaPropertyTable.hpp>
#include <PzaCheckBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaDoubleSpinBox.hpp>
#include <PzaMimeData.hpp>
#include <PzaComboBox.hpp>
#include <PzaPushButton.hpp>

class NoderSPVariable : public PzaWidget
{
    Q_OBJECT

    public:
        NoderSPVariable(QWidget *parent = nullptr);
        
        const QString &name(void) const {return _name;}
        void setName(const QString &name) {_name = name;}

    private:
        QString _name;

    signals:
};

class NoderValBool : public PzaPropertyTable
{
    Q_OBJECT

    public:
        NoderValBool(QWidget *parent = nullptr);

        bool value(void) const {return _value;}

    private:
        bool _value;
        PzaCheckBox *_valueBox;

    private slots:
        void updateValue(int state);
};

class NoderValInt : public PzaPropertyTable
{
    Q_OBJECT

    public:
        NoderValInt(QWidget *parent = nullptr);

        int value(void) const {return _value;}
    
    private:
        int _value;
        PzaSpinBox *_valueBox;

    private slots:
        void updateValue(int value);
};

class NoderValFloat : public PzaPropertyTable
{
    Q_OBJECT
    
    public:
        NoderValFloat(QWidget *parent = nullptr);
        
        double value(void) const {return _value;}

    private:
        double _value;
        PzaDoubleSpinBox *_valueBox;

    private slots:
        void updateValue(double value);
};

class NoderValString : public PzaPropertyTable
{
    Q_OBJECT
    
    public:
        NoderValString(QWidget *parent = nullptr);

        const QString &text(void) const {return _text;}

    private:
        QString _text;
        PzaLineEdit *_textBox;

    private slots:
        void updateText(void);
};

class NoderValEnum : public PzaPropertyTable
{
    Q_OBJECT

    public:
        NoderValEnum(QWidget *parent = nullptr);

        const QString &enumValue(void) {return _enumValue;}
    
    private:
        QString _enumValue;
        PzaComboBox *_enumValuesBox;
    
    private slots:
        void updateNameBox(void);
        void updateValuesBox(void);
    
    signals:
        void enumNameChanged(const QString &name);
};

class NoderValArray : public QWidget
{
    Q_OBJECT

    public:
        NoderValArray(QWidget *parent = nullptr);
};
