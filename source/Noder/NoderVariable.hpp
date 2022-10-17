#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "NoderDataBase.hpp"
#include <PzaLineEdit.hpp>
#include <PzaLabel.hpp>

class NoderVariable : public QWidget
{
    Q_OBJECT

    public:
        NoderVariable(NoderPanel::Type type, QWidget *parent = nullptr);
        NoderVariable(QWidget *parent = nullptr);
        ~NoderVariable() = default;

        void setSelected(bool state);
        const QString &name() {return _name;}
        NoderPanel::Type varType(void) {return _type;}

        void mousePressEvent(QMouseEvent *event) override;

    private:
        QString _name;
        PzaLineEdit *_varLabel;
        QHBoxLayout *_layout;
        QFrame *_colorFrame;
        QColor _varColor;
        NoderPanel::Type _type;

        signals:
            void activate(void);
            void nameChanged(void);
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