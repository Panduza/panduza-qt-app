#include "NoderVariable.hpp"
#include "NoderFunction.hpp"

NoderVariable::NoderVariable(QWidget *parent)
    : QObject(parent)
{

}

void NoderVariable::setFunction(NoderFunction *f)
{
    _function = f;
    if (f)
        f->registerVariable(this);
}

NoderRefBool::NoderRefBool(QWidget *parent)
    : NoderRef(parent)
{
    _props.type = NoderVarProps::Type::Bool;
    _checkBox = _defVal->addRow<PzaCheckBox>("Default value");
}

NoderRefInt::NoderRefInt(QWidget *parent)
    : NoderRef(parent)
{
    _props.type = NoderVarProps::Type::Int;
    _valueBox = _defVal->addRow<PzaSpinBox>("Default value");
    _minBox = _defVal->addRow<PzaSpinBox>("Minimal value");
    _maxBox = _defVal->addRow<PzaSpinBox>("Maximal value");
}

NoderRefFloat::NoderRefFloat(QWidget *parent)
    : NoderRef(parent)
{
    _props.type = NoderVarProps::Type::Float;
    _valueBox = _defVal->addRow<PzaDoubleSpinBox>("Default value");
    _minBox = _defVal->addRow<PzaDoubleSpinBox>("Minimal value");
    _maxBox = _defVal->addRow<PzaDoubleSpinBox>("Maximal value");
    _decimalsBox = _defVal->addRow<PzaSpinBox>("Number of decimals");
}

NoderRefString::NoderRefString(QWidget *parent)
    : NoderRef(parent)
{
    _props.type = NoderVarProps::Type::String;
    _textBox = _defVal->addRow<PzaLineEdit>("Default text");
}

NoderRefEnum::NoderRefEnum(const QString &subType, QWidget *parent)
    : NoderRef(parent)
{
    _props.type = NoderVarProps::Type::Enum;
    _props.subType = subType;
    _enumBox = _defVal->addRow<PzaComboBox>("Default value");
    Noder::ForEachEnumValues(_props.subType, [&](const QString &value) {
        _enumBox->insertItem(0, value);
    });
}

NoderRefInterface::NoderRefInterface(const QString &subType, QWidget *parent)
    : NoderRef(parent)
{
    (void)subType;
    _props.type = NoderVarProps::Type::Interface;
}

NoderRef::NoderRef(QWidget *parent)
    : NoderVariable(parent)
{
    _props.container = NoderVarProps::Container::Reference;
    _defVal = new PzaForm;
}

NoderArray::NoderArray(const NoderVarProps &varProps, QWidget *parent)
    : NoderVariable(parent),
    _nElem(0)
{
    _props = varProps;
    _defVal = new PzaWidget;
    _layout = new QVBoxLayout(_defVal);
    _form = new PzaForm(_defVal);
    _arrayTable = new PzaForm(_defVal);
    _numBox = _form->addRow<PzaSpinBox>("Number of elements");

    _numBox->setMinimum(0);
    _numBox->setMaximum(100);

    connect(_numBox, &PzaSpinBox::valueChanged, this, [&](int val) {
        if (_nElem < val) {
            for (int i = _nElem; i < val; i++) {
                QWidget *widget = addEntry(i);
                /*
                    case NoderVarProps::Type::Enum:
                        widget = new PzaComboBox(_arrayTable);
                        Noder::ForEachEnumValues(_props.subType, [&](const QString &value) {
                            static_cast<PzaComboBox *>(widget)->insertItem(0, value);
                        });
                        break;
                }
*/
                _arrayTable->addRow("[" + QString::number(i) + "]", widget);
            }
        }
        else if (val < _nElem) {
            for (int i = val; i < _nElem; i++) {
                _arrayTable->deleteRow(val);
                removeEntry();
            }
        }
        _nElem = val;
    });

    _layout->addWidget(_form);
    _layout->addWidget(_arrayTable);
}

QWidget *NoderArrayBool::addEntry(int index)
{
    PzaCheckBox *box = new PzaCheckBox();
    connect(box, &PzaCheckBox::stateChanged, this, [&, index](int state)
    {
        _list[index] = (bool)state;
    });

    _list.push_back(false);
    return box;
}

QWidget *NoderArrayInt::addEntry(int index)
{
    PzaSpinBox *box = new PzaSpinBox();
    connect(box, &PzaSpinBox::valueChanged, this, [&, index](int value)
    {
        _list[index] = value;
    });

    _list.push_back(0);
    return box;
}

QWidget *NoderArrayFloat::addEntry(int index)
{
    PzaDoubleSpinBox *box = new PzaDoubleSpinBox();
    connect(box, &PzaDoubleSpinBox::valueChanged, this, [&, index](double value)
    {
        _list[index] = value;
    });

    _list.push_back(0);
    return box;
}

QWidget *NoderArrayString::addEntry(int index)
{
    PzaLineEdit *box = new PzaLineEdit();
    connect(box, &PzaLineEdit::editingFinished, this, [&, box, index]()
    {
        _list[index] = box->text();
    });

    _list.push_back("");
    return box;
}

QWidget *NoderArrayEnum::addEntry(int index)
{
    PzaComboBox *box = new PzaComboBox();

    Noder::ForEachEnumValues(_props.subType, [&](const QString &value) {
        box->insertItem(0, value);
    });

    connect(box, &PzaComboBox::currentIndexChanged, this, [&, box, index]()
    {
        _list[index] = box->currentText();
    });
    
    _list.push_back(box->currentText());
    return box;
}