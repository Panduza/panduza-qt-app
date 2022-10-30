#pragma once

#include <QVBoxLayout>

#include "NoderFrame.hpp"
#include "NoderGraph.hpp"
#include "NoderVariable.hpp"
#include "NoderFunction.hpp"
#include <PzaScrollArea.hpp>
#include <PzaWidget.hpp>
#include <PzaSpoiler.hpp>
#include <PzaMoreLess.hpp>
#include <PzaComboBox.hpp>
#include <PzaPropertyTable.hpp>
#include <PzaPopup.hpp>

#define DEFAULT_FUNCTION_NAME "New Function"

class NoderPropertyArea : public PzaSpoiler
{
    public:
        NoderPropertyArea(QWidget *parent = nullptr);

        void updateProperty(NoderVariable *var);
        void addProperty(NoderVariable *var);
        void deleteProperty(NoderVariable *var);
};

class NoderDefValArea : public PzaSpoiler
{
    public:
        NoderDefValArea(QWidget *parent = nullptr);

        void updateValArea(NoderVariable *var);
        void add(NoderVariable *var);
        void del(NoderVariable *var);
};

class NoderVariableArea : public PzaSpoiler
{
    Q_OBJECT

    public:
        NoderVariableArea(QWidget *parent = nullptr);

        void addVariable(void);
        void removeVariable(void);
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

class NoderFunctionProperties : public PzaPopup
{
    public:
        NoderFunctionProperties(QWidget *parent = nullptr);
    
    private:

};

class NoderFunctionEntry : public PzaWidget
{
    Q_OBJECT

    public:
        NoderFunctionEntry(const QString &name, QWidget *parent = nullptr);
        ~NoderFunctionEntry();

        void setName(const QString &name) {_name->setText(name); _function->setName(name);}
        NoderFunction *function(void) const {return _function;}

    private:
        NoderFunction *_function;
        QHBoxLayout *_layout;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;
        PzaPopup *_propDialog;
        PzaLineEdit *_propName;
        PzaPropertyTable *_propTable;
        PzaLabel *_name;
    
    signals:
        void removed(void);
};

class NoderFunctionArea : public PzaSpoiler
{
    Q_OBJECT

    public:
        NoderFunctionArea(QWidget *parent = nullptr);

    private:
        PzaWidget *_main;
        QVBoxLayout *_layout;
        PzaMoreLess *_moreLess;
        PzaWidget *_functionTable;
        QVBoxLayout *_functionTableLayout;
        //std::map<QString, NoderFunctionEntry *>_functionMap;
        std::vector<NoderFunctionEntry *> _functionList;

        void addFunction(void);
        void removeFunction(void);
};

class NoderSidePanel : public PzaScrollArea
{
    Q_OBJECT

    public:
        NoderSidePanel(NoderSidePanel &other) = delete;
        void operator=(const NoderSidePanel &) = delete;

        NoderVariableArea *VariableArea = nullptr;
        NoderFunctionArea *FunctionArea = nullptr;

        static NoderSidePanel *Get(void)
        {
            static NoderSidePanel *sidePanel;

            if (sidePanel == nullptr)
                sidePanel = new NoderSidePanel();
            return sidePanel;
        }

    private:
        NoderSidePanel(QWidget *parent = nullptr);
        
        PzaWidget *_main;
        QVBoxLayout *_layout;
};