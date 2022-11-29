#include "NoderSPVarPicker.hpp"

NoderSPVarPicker::NoderSPVarPicker(QWidget *parent)
    : PzaWidget(parent)
{
    _typeDrop = new PzaToolButton(this);
    _ctnDrop = new PzaComboBox(this);
    _layout = new QHBoxLayout(this);

    _layout->setContentsMargins(0, 0, 0, 0);
    _ctnDrop->setIconSize(QSize(8, 8));
    
    DEFAULT_VARIABLE_VAR(_varProps);

    Noder::Get().ForEachVarContainer([&](NoderVarProps::Container ctn) {
        _ctnDrop->insertItem(0, Noder::VarContainerName(ctn));
    });
    connect(_ctnDrop, &PzaComboBox::currentIndexChanged, this, [&](int index) {
        struct NoderVarProps &v = _varProps;
        v.container = Noder::VarContainerFromName(_ctnDrop->itemText(index));
        setVar(v);
    });

    _typeDrop->setText(Noder::VarTypeName(_varProps.type));

    configureTree();

    _layout->addWidget(_ctnDrop);
    _layout->addWidget(_typeDrop);
}

void NoderSPVarPicker::setVar(const NoderVarProps &varProps)
{
    setContainer(varProps.container);
    setType(varProps.type, varProps.subType);
}

void NoderSPVarPicker::setType(const NoderVarProps::Type type, const QString &subType)
{
    _varProps.type = type;
    _varProps.subType = subType;
    refreshContainerIco();
    _typeDrop->setIcon(Noder::PlugContainerFunc(_varProps.container)(_varProps.type, false));
    (Noder::VarHasSubType(_varProps.type)) ? _typeDrop->setText(_varProps.subType) : _typeDrop->setText(Noder::VarTypeName(_varProps.type));
    varChanged(_varProps);
}

void NoderSPVarPicker::setContainer(const NoderVarProps::Container ctn)
{
    _varProps.container = ctn;
    refreshTypeIco();
    _ctnDrop->blockSignals(true);
    _ctnDrop->setCurrentText(Noder::VarContainerName(ctn));
    _ctnDrop->blockSignals(false);
    varChanged(_varProps);
}

void NoderSPVarPicker::configureTree(void)
{
    _typeDrop->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    PzaMenu *menu = new PzaMenu(this);

    QWidgetAction *searchBarAction = new QWidgetAction(this);
    PzaLineEdit *searchBar = new PzaLineEdit(this);
    searchBarAction->setDefaultWidget(searchBar);
    
    QWidgetAction *treeAction = new QWidgetAction(this);
    PzaTreeWidget *tree = new PzaTreeWidget(this);
    treeAction->setDefaultWidget(tree);
    tree->setHeaderHidden(true);

    searchBar->setPlaceholderText("Search");

    _variableCat = addCategory("Variables", tree);
    _interfaceCat = addCategory("Interfaces", tree);
    _enumCat = addCategory("Enums", tree);

    tree->setIconSize(QSize(8, 8));

    _variableCat->setExpanded(true);

    Noder::Get().ForEachVarType([&](NoderVarProps::Type type) {
        if (Noder::Get().VarTypeProperties(type).canBeInstance) {
            PzaTreeWidgetItem *item = addItem(Noder::Get().VarTypeName(type));
            _variableCat->insertChild(0, item);
            item->setSvgIcon(0, Noder::PlugValue(type, false));
        }
    });

    Noder::Get().ForEachEnumName([&](const QString &name) {
        PzaTreeWidgetItem *item = addItem(name, _enumCat);
        item->setSvgIcon(0, Noder::PlugValue(NoderVarProps::Type::Enum, false));
    });

    connect(searchBar, &PzaLineEdit::textChanged, this, [&](const QString &s){
        QTreeWidgetItemIterator it(tree);

        _variableCat->setExpanded(true);
        _interfaceCat->setExpanded(true);
        _enumCat->setExpanded(true);

        while (*it) {
            PzaTreeWidgetItem *item = static_cast<PzaTreeWidgetItem *>(*it);
            if (item->isCategory() == false) {
                bool match = item->text(0).contains(s, Qt::CaseInsensitive);
                (match) ? item->setHidden(false) : item->setHidden(true);
            }
            it++;
        }
    });

    connect(tree, &QTreeWidget::itemClicked, this, [&, menu](QTreeWidgetItem *from,  int column) {
        (void)column;
        PzaTreeWidgetItem *item = static_cast<PzaTreeWidgetItem *>(from);
        PzaTreeWidgetItem *parent;

        if (item->isCategory() == false) {
            parent =  static_cast<PzaTreeWidgetItem *>(item->parent());
            _varProps.subType = item->text(0);
            if (parent->text(0) == "Enums") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_varProps.container)(NoderVarProps::Type::Enum, false));
                setType(NoderVarProps::Type::Enum, item->text(0));
            }
            else if (parent->text(0) == "Interfaces") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_varProps.container)(NoderVarProps::Type::Interface, false));
                setType(NoderVarProps::Type::Interface, item->text(0));
            }
            else {
                NoderVarProps::Type type = Noder::Get().VarTypeFromName(item->text(0));
                _typeDrop->setIcon(Noder::PlugContainerFunc(_varProps.container)(type, false));
                setType(type);
            }
            menu->close();
        }
    });

    menu->addAction(searchBarAction);
    menu->addAction(treeAction);

    _typeDrop->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    _typeDrop->setIconSize(QSize(8, 8));
    _typeDrop->setPopupMode(QToolButton::InstantPopup);
    _typeDrop->setMenu(menu);
}

void NoderSPVarPicker::refreshContainerIco(void)
{
    QPixmap pixmap;

    pixmap.loadFromData(Noder::PlugValue(_varProps.type, false));
    _ctnDrop->setItemIcon(0, pixmap);

    pixmap.loadFromData(Noder::PlugArray(_varProps.type, false));
    _ctnDrop->setItemIcon(1, pixmap);
}

void NoderSPVarPicker::refreshTypeIco(void)
{
    for (int i = 0; i < _variableCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_variableCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_varProps.container)(Noder::VarTypeFromName(child->text(0)), false));
    }

    for (int i = 0; i < _enumCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_enumCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_varProps.container)(NoderVarProps::Type::Enum, false));
    }

    _typeDrop->setIcon(Noder::PlugContainerFunc(_varProps.container)(_varProps.type, false));
}

PzaTreeWidgetItem *NoderSPVarPicker::addItem(const QString &name, PzaTreeWidgetItem *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);
    
    item->setText(0, name);
    return item;
}

PzaTreeWidgetItem *NoderSPVarPicker::addCategory(const QString &name, PzaTreeWidget *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);

    item->setText(0, name);
    item->setCategory(true);
    return item;
}