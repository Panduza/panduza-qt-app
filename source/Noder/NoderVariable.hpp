#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDrag>

#include "NoderDataBase.hpp"
#include <PzaWidget.hpp>
#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>
#include <PzaPropertyTable.hpp>
#include <PzaCheckBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaDoubleSpinBox.hpp>
#include <PzaMimeData.hpp>
#include <PzaComboBox.hpp>

class NoderVariable : public PzaWidget
{
    Q_OBJECT

    public:
        NoderVariable(NoderPanel::Type type, QWidget *parent = nullptr);
        ~NoderVariable() = default;
        void setSelected(bool state);
        const QString &name() {return _name;}
        NoderPanel::Type type(void) {return _type;}
        void setName(const QString &name);
        void setType(const NoderPanel::Type type);
        PzaPropertyTable *propTable(void) {return _propTable;}
        PzaPropertyTable *defValTable(void) {return _defValTable;}
        void createDefValTable(void);

        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
    
    private:
        QString _name;
        PzaLineEdit *_varLabel;
        QHBoxLayout *_layout;
        QFrame *_colorFrame;
        NoderPanel::Type _type;

        PzaPropertyTable *_propTable;
        PzaLabel *_propName = nullptr;
        PzaLabel *_typeName = nullptr;
        PzaComboBox *_propType;

        PzaPropertyTable *_defValTable = nullptr;

    signals:
        void activate(void);
        void typeChanged(void);
        void nameChanged(const QString &name);
};

class NoderValBool : public PzaPropertyTable
{
    Q_OBJECT

    public:
        NoderValBool(QWidget *parent = nullptr);
        ~NoderValBool() = default;

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
        ~NoderValInt() = default;

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
        ~NoderValFloat() = default;
        
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
        ~NoderValString() = default;

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
        ~NoderValEnum() = default;

        const QString &enumName(void) {return _enumName;}
        const QString &enumValue(void) {return _enumValue;}
    
    private:
        QString _enumName;
        QString _enumValue;
        PzaComboBox *_enumNameBox;
        PzaComboBox *_enumValuesBox;
    
    private slots:
        void updateNameBox(void);
        void updateValuesBox(void);
    
    signals:
        void enumNameChanged(const QString &name);
};
