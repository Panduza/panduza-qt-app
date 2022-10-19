#pragma once

#include <QVBoxLayout>
#include <QScrollArea>
#include <PzaWidget.hpp>
#include <PzaSpoiler.hpp>
#include <PzaMoreLess.hpp>
#include "NoderVariable.hpp"
#include <PzaComboBox.hpp>
#include <PzaPropertyTable.hpp>

class NoderPropertyArea : public PzaSpoiler
{
    public:
        NoderPropertyArea(QWidget *parent = nullptr);
        ~NoderPropertyArea() = default;

        void updateProperty(NoderVariable *var);
        void addProperty(NoderVariable *var);
        void deleteProperty(NoderVariable *var);
};

class NoderDefValArea : public PzaSpoiler
{
    public:
        NoderDefValArea(QWidget *parent = nullptr);
        ~NoderDefValArea() = default;

        void updateValArea(NoderVariable *var);
        void add(NoderVariable *var);
        void del(NoderVariable *var);
};

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
        PzaWidget *_varTable;
        QVBoxLayout *_varTableLayout;
        PzaMoreLess *_moreLess;
        std::vector<NoderVariable *> _varList;
        NoderVariable *_selectedVar = nullptr;
        NoderPropertyArea *_propertyArea;
        NoderDefValArea *_defValArea;

    signals:
        void varUpdated(NoderVariable *var);
        void varAdded(NoderVariable *var);
        void varRemoved(NoderVariable *var);
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