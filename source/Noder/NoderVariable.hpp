#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "NoderDataBase.hpp"
#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>
#include <PzaPropertyTable.hpp>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

class NoderVariable : public QWidget
{
    Q_OBJECT

    public:
        NoderVariable(NoderPanel::Type type, QWidget *parent = nullptr);
        ~NoderVariable() = default;
        void setSelected(bool state);
        const QString &name() {return _name;}
        void setName(const QString &name);
        void setType(const NoderPanel::Type type);
        NoderPanel::Type varType(void) {return _type;}
        PzaPropertyTable *propTable(void) {return _propTable;}
        PzaPropertyTable *defValTable(void) {return _defValTable;}
        void createDefValTable(void);

        void mousePressEvent(QMouseEvent *event) override;
    
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
};

class NoderValBool : public PzaPropertyTable
{
    public:
        NoderValBool(QWidget *parent = nullptr);
        ~NoderValBool() = default;

    private:
        QCheckBox *_default;
};

class NoderValInt : public PzaPropertyTable
{
    public:
        NoderValInt(QWidget *parent = nullptr);
        ~NoderValInt() = default;

    private:
        QSpinBox *_default;
};

class NoderValFloat : public PzaPropertyTable
{
    public:
        NoderValFloat(QWidget *parent = nullptr);
        ~NoderValFloat() = default;

    private:
        QDoubleSpinBox *_default;
};

class NoderValString : public PzaPropertyTable
{
    public:
        NoderValString(QWidget *parent = nullptr);
        ~NoderValString() = default;

    private:
        PzaLineEdit *_default;
};

class NoderValEnum : public PzaPropertyTable
{
    public:
        NoderValEnum(QWidget *parent = nullptr);
        ~NoderValEnum() = default;
    
    private:
        PzaComboBox *_enumName;
};
