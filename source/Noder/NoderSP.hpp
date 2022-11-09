#pragma once

#include <QVBoxLayout>

#include "NoderGraph.hpp"
#include "NoderSPVariable.hpp"
#include "NoderFunction.hpp"
#include <PzaScrollArea.hpp>
#include <PzaWidget.hpp>
#include <PzaSpoiler.hpp>
#include <PzaMoreLess.hpp>
#include <PzaComboBox.hpp>
#include <PzaPropertyTable.hpp>
#include <PzaToolButton.hpp>
#include <PzaPopup.hpp>
#include <PzaTreeWidget.hpp>
#include <PzaTreeWidgetItem.hpp>
#include <PzaNameEntry.hpp>

#define DEFAULT_FUNCTION_NAME       "New Function"
#define DEFAULT_PIN_NAME            "New Pin"
#define DEFAULT_VARIABLE_NAME       "New Variable"
#define DEFAULT_VARIABLE_TYPE       NoderVar::Type::Bool
#define DEFAULT_VARIABLE_CONTAINER  NoderVar::Container::Variable
#define DEFAULT_PIN_DIRECTION       PinProperty::Direction::Input
#define DEFAULT_PIN_CONTAINER       NoderVar::Container::Variable
#define DEFAULT_PIN_TYPE            NoderVar::Type::Bool

enum SPVarGrid
{
    NAME,
    CONTAINER,
    TYPE,
    PROP,
    DELETE
};

class NoderFunctionPinArea;

class NoderSPDirBox : public PzaComboBox
{
    Q_OBJECT
    
    public:
        NoderSPDirBox(QWidget *parent = nullptr);
        void setDirection(PinProperty::Direction dir);
    
    signals:
        void directionChanged(PinProperty::Direction dir);
};

class NoderPin : public QObject
{
    Q_OBJECT

    public:
        void setName(const QString &name) {_name = name;}
        void setContainer(NoderVar::Container ctn) {_ctn = ctn;}
        void setType(NoderVar::Type type) {_type = type;}
        void setDirection(PinProperty::Direction direction) {_direction = direction;}
        void setPin(PinValue *pin) {_pin = pin;}

        const QString &name(void) const {return _name;}
        NoderVar::Container container(void) const {return _ctn;}
        NoderVar::Type type(void) const {return _type;}
        PinProperty::Direction direction(void) const {return _direction;}
        PinValue *pin(void) const {return _pin;}

    private:
        QString _name;
        NoderVar::Type _type;
        NoderVar::Container _ctn;
        PinProperty::Direction _direction;
        PinValue *_pin = nullptr;
};

class NoderSPVarDrop : public PzaWidget
{
    Q_OBJECT

    public:
        NoderSPVarDrop(QWidget *parent = nullptr);

        void setType(NoderVar::Type type);
        NoderVar::Type type(void) const {return _type;}
        void setContainer(NoderVar::Container ctn);
        NoderVar::Container container(void) const {return _ctn;}

    private:
        PzaTreeWidgetItem *_variableCat;
        PzaTreeWidgetItem *_interfaceCat;
        PzaTreeWidgetItem *_enumCat;
        PzaToolButton *_typeDrop;
        PzaComboBox *_ctnDrop;
        NoderVar::Type _type;
        NoderVar::Container _ctn;
        QHBoxLayout *_layout;

        PzaTreeWidgetItem *addItem(const QString &name, PzaTreeWidgetItem *parent = nullptr);
        PzaTreeWidgetItem *addCategory(const QString &name, PzaTreeWidget *parent = nullptr);
        void refreshContainerIco(void);
        void refreshTypeIco(void);
        void configureTree(void);
    
    signals:
        void varChanged(NoderVar::Container ctn, NoderVar::Type type);
};

template <class N>
class NoderSPEntry : public PzaWidget
{
    public:
        virtual void setName(const QString &name);
        const QString &name(void) {return _name;}
        virtual void setSelected(bool state);
        void setElem(N *elem) {_elem = elem;}
        N *elem(void) const {return _elem;}

    protected:
        NoderSPEntry(QWidget *parent = nullptr);

        virtual void remove(void) = 0;

        N *_elem;
        QString _name;
        QHBoxLayout *_layout;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;
        PzaNameEntry *_nameEntry;
};

class NoderPinEntry : public NoderSPEntry<PinValue>
{
    Q_OBJECT
    
    public:
        NoderPinEntry(QWidget *parent = nullptr);

        void setContainer(NoderVar::Container ctn);
        void setType(NoderVar::Type type);
        void setDirection(PinProperty::Direction direction);

        NoderVar::Container container(void) const {return _ctn;}
        NoderVar::Type type(void) const {return _type;}
        PinProperty::Direction direction(void) const {return _direction;}
        
        void remove(void) override {removed();}

    private:
        NoderSPVarDrop *_varDrop;
        NoderVar::Container _ctn;
        NoderVar::Type _type;
        PinProperty::Direction _direction;
        NoderSPDirBox *_dirBox;

    signals:
        void varChanged(void);
        void directionChanged(void);
        void removed(void);
};


template <class N>
class NoderSPArea : public PzaWidget
{
    public:
        std::vector<N *> &entryList(void) {return _entryList;}

    protected:
        NoderSPArea(QWidget *parent = nullptr);

        virtual void addEntry(void);
        virtual void removeEntry(N *target);
        virtual void selectEntry(N *target);

        PzaPushButton *_addBtn;
        QVBoxLayout *_layout;
        PzaWidget *_entryTable;
        QVBoxLayout *_entryTableLayout;
        std::vector<N *> _entryList;
        N *_selectedEntry = nullptr;
        QString _defaultEntryName;
};

class NoderPinArea : public NoderSPArea<NoderPinEntry>
{
    Q_OBJECT
    
    public:
        NoderPinArea(QWidget *parent = nullptr);

        void addEntry(void) override;
        void removeEntry(NoderPinEntry *target) override;
        void selectEntry(NoderPinEntry *target) override;

    signals:
        void pinRemoved(NoderPinEntry *elem);
        void varChanged(NoderPinEntry *elem);
        void directionChanged(NoderPinEntry *elem);
};


class NoderFunctionEntry : public NoderSPEntry<NoderFunction>
{
    Q_OBJECT
    
    public:
        NoderFunctionEntry(QWidget *parent = nullptr);
        
        void remove(void) override {removed();}
        NoderPinArea *pinArea(void) const {return _pinArea;}
        void createPinArea(void);
        void updateVar(NoderPinEntry *pin);
        void updateDirection(NoderPinEntry *pin);
        void removePin(NoderPinEntry *pin);

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        NoderPinArea *_pinArea = nullptr;

    signals:
        void removed(void);
};

class NoderSPVariableEntry : public NoderSPEntry<PzaWidget>
{
    Q_OBJECT

    public:
        NoderSPVariableEntry(QWidget *parent = nullptr);

        void remove(void) override {removed();}
        void setContainer(NoderVar::Container ctn);
        NoderVar::Container container(void) const {return _ctn;}
        void setType(NoderVar::Type type);
        NoderVar::Type type() const {return _type;}

    private:
        NoderVar::Container _ctn;
        NoderVar::Type _type;
        NoderSPVarDrop *_varDrop;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;

    signals:
        void removed(void);
        
};

class NoderSPVariableArea : public NoderSPArea<NoderSPVariableEntry>
{
    public:
        NoderSPVariableArea(QWidget *parent = nullptr);

        void addEntry(void);
        void selectEntry(NoderSPVariableEntry *entry);
        void removeEntry(NoderSPVariableEntry *entry);

    private:
        PzaSpoiler *_defValArea = nullptr;
};

class NoderFunctionArea : public NoderSPArea<NoderFunctionEntry>
{
    public:
        NoderFunctionArea(QWidget *parent = nullptr);

        QJsonArray save(void);

        void addEntry(void) override;
        void removeEntry(NoderFunctionEntry *target) override;
        void selectEntry(NoderFunctionEntry *target) override;

    private:
        PzaSpoiler *_pinSpoiler;
};

class NoderSP : public PzaScrollArea
{
    Q_OBJECT

    friend class NoderFrame;

    public:
        NoderSP(NoderSP &other) = delete;
        void operator=(const NoderSP &) = delete;

        NoderSPVariableArea *VariableArea = nullptr;
        NoderFunctionArea *FunctionArea = nullptr;

        void save();

    private:
        NoderSP(QWidget *parent = nullptr);
        
        PzaWidget *_main;
        QVBoxLayout *_layout;
};