#pragma once

#include <QWidget>
#include <PzaWidget.hpp>
#include <PzaForm.hpp>
#include <PzaCheckBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaDoubleSpinBox.hpp>
#include <PzaComboBox.hpp>
#include <PzaLineEdit.hpp>
#include "Noder.hpp"

class NoderFunction;

class NoderVariable : public QObject
{
    Q_OBJECT
    
    public:
        NoderVariable(QWidget *parent = nullptr);
        
        void setName(const QString &name) {_name = name; nameChanged(name);}
        const QString &name(void) const {return _name;}
        void setType(NoderVarProps::Type type) {_props.type = type;}
        const NoderVarProps &props(void) const {return _props;}
        virtual PzaWidget *defVal(void) {return nullptr;}
        void setGlobal(bool state) {_isGlobal = state;}
        bool isGlobal(void) const {return _isGlobal;}
        void setFunction(NoderFunction *f);
    
        QString _name;
        NoderVarProps _props;
        bool _isGlobal;
        NoderFunction *_function;
    
    signals:
        void nameChanged(const QString &name);
};

class NoderRef : public NoderVariable
{
    public:
        NoderRef(QWidget *parent = nullptr);
        
        void refreshDefVal(NoderVarProps::Container ctn, NoderVarProps::Type type);
        PzaWidget *defVal(void) override {return _defVal;}

    protected:
        PzaForm *_defVal = nullptr;
};

class NoderRefBool : public NoderRef
{
    Q_OBJECT

    public:
        NoderRefBool(QWidget *parent = nullptr);

        void setValue(bool value) { _value = value; }
        bool value(void) const { return _value; }

    private:
        bool _value;
        PzaCheckBox *_checkBox;
};

class NoderRefInt : public NoderRef
{
    Q_OBJECT

    public:
        NoderRefInt(QWidget *parent = nullptr);

        void setValue(int value) {_value = value;}
        void setMin(int min) {_min = min;}
        void setMax(int max) {_max = max;}
        int value(void) const {return _value;}
        int min(void) const {return _min;}
        int max(void) const {return _max;}
    
    private:
        int _value;
        int _min;
        int _max;
        PzaSpinBox *_valueBox;
        PzaSpinBox *_minBox;
        PzaSpinBox *_maxBox;
};

class NoderRefFloat : public NoderRef
{
    Q_OBJECT
    
    public:
        NoderRefFloat(QWidget *parent = nullptr);

        void setValue(double value) {_value = value;}
        void setMin(double min) {_min = min;}
        void setMax(double max) {_max = max;}
        double value(void) const {return _value;}
        double min(void) const {return _min;}
        double max(void) const {return _max;}
        double decimals(void) const {return _decimals;}

    private:
        double _value;
        double _min;
        double _max;
        unsigned int _decimals;
        PzaDoubleSpinBox *_valueBox;
        PzaDoubleSpinBox *_minBox;
        PzaDoubleSpinBox *_maxBox;
        PzaSpinBox *_decimalsBox;
};

class NoderRefString : public NoderRef
{
    Q_OBJECT
    
    public:
        NoderRefString(QWidget *parent = nullptr);

        void setValue(const QString &value) {_value = value;}
        const QString &value(void) const {return _value;}

    private:
        QString _value;
        PzaLineEdit *_textBox;
};

class NoderRefEnum : public NoderRef
{
    Q_OBJECT

    public:
        NoderRefEnum(const QString &subType, QWidget *parent = nullptr);
    
    private:
        PzaComboBox *_enumBox;
};

class NoderRefInterface : public NoderRef
{
    Q_OBJECT

    public:
        NoderRefInterface(const QString &subType, QWidget *parent = nullptr);
};

class NoderArray : public NoderVariable
{
    public:
        NoderArray(const NoderVarProps &varProps, QWidget *parent = nullptr);

        virtual QWidget *addEntry(int index) = 0;
        virtual void removeEntry(void) = 0;

        PzaWidget *defVal(void) override {return _defVal;}

    protected:
        int _nElem;
        PzaForm *_form;
        PzaForm *_arrayTable;
        PzaSpinBox *_numBox;
        PzaWidget *_defVal = nullptr;
        QVBoxLayout *_layout = nullptr;
};

class NoderArrayBool : public NoderArray
{
    public:
        NoderArrayBool(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override;
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<bool> _list;
};

class NoderArrayInt : public NoderArray
{
    public:
        NoderArrayInt(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override;
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<int> _list;
};

class NoderArrayFloat : public NoderArray
{
    public:
        NoderArrayFloat(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override;
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<double> _list;
};

class NoderArrayString : public NoderArray
{
    public:
        NoderArrayString(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override;
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<QString> _list;
};

class NoderArrayEnum : public NoderArray
{
    public:
        NoderArrayEnum(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override;
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<QString> _list;
};

class NoderArrayInterface : public NoderArray
{
    public:
        NoderArrayInterface(const NoderVarProps &varProps, QWidget *parent = nullptr)
            : NoderArray(varProps, parent)
        {

        }

        QWidget *addEntry(int index) override {(void)index; return nullptr;};
        void removeEntry(void) override {_list.pop_back();}

    protected:
        std::vector<void *> _list;
};