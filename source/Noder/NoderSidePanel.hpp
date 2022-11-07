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

#define DEFAULT_FUNCTION_NAME   "New Function"
#define DEFAULT_PIN_NAME        "New Pin"
#define DEFAULT_VARIABLE_NAME   "New Variable"
#define DEFAULT_VARIABLE_TYPE   NoderPanel::Type::Bool
#define DEFAULT_PIN_TYPE        PinProperty::Type::Bool
#define DEFAULT_PIN_DIRECTION   PinProperty::Direction::Input

class NoderFunctionPinArea;

class NoderPin : public QObject
{
    Q_OBJECT

    public:
        void setName(const QString &name) {_name = name;}
        void setType(PinProperty::Type type) {_type = type;}
        void setDirection(PinProperty::Direction direction) {_direction = direction;}
        void setPin(Pin *pin) {_pin = pin;}

        const QString &name(void) const {return _name;}
        PinProperty::Type type(void) const {return _type;}
        PinProperty::Direction direction(void) const {return _direction;}
        Pin *pin(void) const {return _pin;}

    private:
        QString _name;
        PinProperty::Type _type;
        PinProperty::Direction _direction;
        Pin *_pin = nullptr;
};

template <class N>
class NoderSidePanelEntry : public PzaWidget
{
    public:
        virtual void setName(const QString &name);
        const QString &name(void) {return _name;}
        virtual void setSelected(bool state);
        N *elem(void) const {return _elem;}

    protected:
        NoderSidePanelEntry(QWidget *parent = nullptr);

        virtual void remove(void) {}

        N *_elem;
        QString _name;
        QHBoxLayout *_layout;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;
        PzaLabel *_nameLabel;
        PzaPopup *_propPopup;
        PzaPropertyTable *_propTable;
        PzaLineEdit *_propName;
};

class NoderVariableEntry : public NoderSidePanelEntry<NoderVariable>
{
    Q_OBJECT

    public:
        NoderVariableEntry(QWidget *parent = nullptr);

        void remove(void) override {removed();}
        void setType(NoderPanel::Type type);
        PzaPropertyTable *createDefValTable(void);

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        QFrame *_colorFrame;
        PzaLabel *_typeLabel;
        PzaComboBox *_propType;
    
    signals:
        void typeChanged(void);
        void removed(void);
};


class NoderPinEntry : public NoderSidePanelEntry<NoderPin>
{
    Q_OBJECT
    
    public:
        NoderPinEntry(QWidget *parent = nullptr);

        void setType(PinProperty::Type type);
        void setDirection(PinProperty::Direction direction);
        void setName(const QString &name) override;
        
        void remove(void) override {removed();}

    private:
        QFrame *_colorFrame;
        PzaLabel *_typeLabel;
        PzaLabel *_directionLabel;
        PzaComboBox *_propType;
        PzaComboBox *_propDirection;

    signals:
        void typeChanged(void);
        void directionChanged(void);
        void removed(void);
};


class NoderFunctionEntry : public NoderSidePanelEntry<NoderFunction>
{
    Q_OBJECT
    
    public:
        NoderFunctionEntry(QWidget *parent = nullptr);
        
        void remove(void) override {removed();}
        NoderFunctionPinArea *pinArea(void) const {return _pinArea;}
        void createPinArea(void);
        void updateType(NoderPin *pin);
        void updateDirection(NoderPin *pin);
        void removePin(NoderPin *pin);

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        NoderFunctionPinArea *_pinArea = nullptr;

    signals:
        void removed(void);
};

template <class N>
class NoderSidePanelArea : public PzaWidget
{
    protected:
        NoderSidePanelArea(QWidget *parent = nullptr);

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

class NoderFunctionPinArea : public NoderSidePanelArea<NoderPinEntry>
{
    Q_OBJECT
    
    public:
        NoderFunctionPinArea(QWidget *parent = nullptr);

        void addEntry(void) override;
        void removeEntry(NoderPinEntry *target) override;
        void selectEntry(NoderPinEntry *target) override;

    signals:
        void pinRemoved(NoderPin *elem);
        void typeChanged(NoderPin *elem);
        void directionChanged(NoderPin *elem);
};

class NoderVariableArea : public NoderSidePanelArea<NoderVariableEntry>
{
    public:
        NoderVariableArea(QWidget *parent = nullptr);

        QJsonArray save(void);
        
        void addEntry(void) override;
        void removeEntry(NoderVariableEntry *target) override;
        void selectEntry(NoderVariableEntry *target) override;

    private:
        PzaSpoiler *_defValArea = nullptr;
};

class NoderFunctionArea : public NoderSidePanelArea<NoderFunctionEntry>
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

        void save();

    private:
        NoderSidePanel(QWidget *parent = nullptr);
        
        PzaWidget *_main;
        QVBoxLayout *_layout;
};