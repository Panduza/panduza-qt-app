#pragma once

#include <QVBoxLayout>
#include <QScrollArea>
#include <PzaWidget.hpp>
#include <PzaSpoiler.hpp>
#include <PzaMoreLess.hpp>
#include "NoderVariable.hpp"
#include <PzaComboBox.hpp>
#include <PzaPropertyTable.hpp>

class NoderVarArea : public PzaSpoiler
{
    Q_OBJECT

    public:
        NoderVarArea(QWidget *parent = nullptr);
        ~NoderVarArea() = default;

        void addVariable(void);
        void removeVariable(NoderVariable *target);
        void selectVar(NoderVariable *target);

    private:
        PzaWidget *_main;
        QVBoxLayout *_layout;
        PzaMoreLess *_moreLess;
        std::vector<NoderVariable *> _varList;
        NoderVariable *_selectedVar = nullptr;

    signals:
        void updated(NoderVariable *var);
};

class NoderPropertyArea : public PzaSpoiler
{
    Q_OBJECT

    public:
        NoderPropertyArea(QWidget *parent = nullptr);
        ~NoderPropertyArea() = default;

    private:
        PzaPropertyTable *_propertyTable;
        PzaLabel *_propName = nullptr;
        PzaComboBox *_propType;

    public slots:
        void updateProperties(NoderVariable *var);
};

class NoderSidePanel : public QScrollArea
{
    public:
        NoderSidePanel(QWidget *parent = nullptr);
        ~NoderSidePanel() = default;

    private:
        QWidget *_main;
        QVBoxLayout *_layout;
        PzaSpoiler *_varSp;
        NoderVarArea *_varArea;
        NoderPropertyArea *_propertyArea;
};