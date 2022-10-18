#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "NoderDataBase.hpp"
#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>
#include <PzaPropertyTable.hpp>

class NoderVariable : public QWidget
{
    Q_OBJECT

    public:
        NoderVariable(NoderPanel::Type type, QWidget *parent = nullptr);
        NoderVariable(QWidget *parent = nullptr);
        ~NoderVariable() = default;

        void setSelected(bool state);
        const QString &name() {return _name;}
        void setName(const QString &name);
        void setType(const NoderPanel::Type type);
        NoderPanel::Type varType(void) {return _type;}
        PzaPropertyTable *propTable(void) {return _propTable;}
        QString propName(void) {return _propName->text();}

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

        signals:
            void activate(void);
};

/*
class NoderVarBool : public NoderVar
{
    protected:
        NoderVarBool(QWidget *parent = nullptr);
        ~NoderVarBool() = default;
};

class NoderVarFloat : public NoderVar
{
    protected:
        NoderVarFloat(QWidget *parent = nullptr);
        ~NoderVarFloat() = default;
};

class NoderVarString : public NoderVar
{
    protected:
        NoderVarString(QWidget *parent = nullptr);
        ~NoderVarString() = default;
};

class NoderVarEnum : public NoderVar
{
    protected:
        NoderVarEnum(QWidget *parent = nullptr);
        ~NoderVarEnum() = default;
};
*/