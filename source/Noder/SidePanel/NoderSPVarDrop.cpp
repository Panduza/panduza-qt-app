#include "NoderSPVarDrop.hpp"

NoderSPVarDrop::NoderSPVarDrop(QWidget *parent)
    : PzaWidget(parent)
{
    _typeDrop = new PzaToolButton(this);
    _ctnDrop = new PzaComboBox(this);
    _layout = new QHBoxLayout(this);

    _layout->setContentsMargins(0, 0, 0, 0);

    _ctn = DEFAULT_VARIABLE_CONTAINER;
    _type = DEFAULT_VARIABLE_TYPE;

    _ctnDrop->addItem("Variable");
    _ctnDrop->addItem("Array");
    _ctnDrop->setIconSize(QSize(8, 8));
    connect(_ctnDrop, &PzaComboBox::currentIndexChanged, this, [&](int index) {
        NoderVar::Container ctn = Noder::varContainerFromName(_ctnDrop->itemText(index));
        
        setContainer(ctn);
    });

    configureTree();

    _layout->addWidget(_ctnDrop);
    _layout->addWidget(_typeDrop);
}

void NoderSPVarDrop::setContainer(NoderVar::Container ctn)
{
    _ctn = ctn;
    refreshTypeIco();

    _ctnDrop->blockSignals(true);
    _ctnDrop->setCurrentText(Noder::varContainerName(ctn));
    _ctnDrop->blockSignals(false);
    varChanged(_ctn, _type);
}

void NoderSPVarDrop::setType(NoderVar::Type type)
{
    _type = type;
    refreshContainerIco();
    _typeDrop->setText(Noder::varTypeName(type));
    _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(type, false));
    varChanged(_ctn, _type);
}

void NoderSPVarDrop::configureTree(void)
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

    Noder::Get().forEachVarType([&](NoderVar::Type type) {
        if (Noder::Get().varTypeProperties(type).canBeInstance) {
            PzaTreeWidgetItem *item = addItem(Noder::Get().varTypeName(type));
            _variableCat->insertChild(0, item);
            item->setSvgIcon(0, Noder::PlugValue(type, false));
        }
    });

    Noder::Get().forEachEnumName([&](const QString &name) {
        PzaTreeWidgetItem *item = addItem(name, _enumCat);
        item->setSvgIcon(0, Noder::PlugValue(NoderVar::Type::Enum, false));
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
            if (parent->text(0) == "Enums") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Enum, false));
                setType(NoderVar::Type::Enum);
            }
            else if (parent->text(0) == "Interfaces") {
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Interface, false));
                setType(NoderVar::Type::Interface);
            }
            else {
                NoderVar::Type type = Noder::Get().varTypeFromName(item->text(0));
                _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(type, false));
                setType(type);
            }
            _typeDrop->setText(item->text(0));
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

void NoderSPVarDrop::refreshContainerIco(void)
{
    QPixmap pixmap;

    pixmap.loadFromData(Noder::PlugValue(_type, false));
    _ctnDrop->setItemIcon(0, pixmap);

    pixmap.loadFromData(Noder::PlugArray(_type, false));
    _ctnDrop->setItemIcon(1, pixmap);
}

void NoderSPVarDrop::refreshTypeIco(void)
{
    for (int i = 0; i < _variableCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_variableCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_ctn)(Noder::varTypeFromName(child->text(0)), false));
    }

    for (int i = 0; i < _enumCat->childCount(); i++) {
        PzaTreeWidgetItem *child = static_cast<PzaTreeWidgetItem *>(_enumCat->child(i));
        child->setSvgIcon(0, Noder::PlugContainerFunc(_ctn)(NoderVar::Type::Enum, false));
    }

    _typeDrop->setIcon(Noder::PlugContainerFunc(_ctn)(_type, false));
}

PzaTreeWidgetItem *NoderSPVarDrop::addItem(const QString &name, PzaTreeWidgetItem *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);
    
    item->setText(0, name);
    return item;
}

PzaTreeWidgetItem *NoderSPVarDrop::addCategory(const QString &name, PzaTreeWidget *parent)
{
    PzaTreeWidgetItem *item = new PzaTreeWidgetItem(parent);

    item->setText(0, name);
    item->setCategory(true);
    return item;
}