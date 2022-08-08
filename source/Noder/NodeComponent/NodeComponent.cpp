#include <NodeComponent.hpp>
#include <Node.hpp>

NodeComponent::NodeComponent(QString name, Node *parent)
    : QWidget(),
    _node(parent)
{
    _proxy = new QGraphicsProxyWidget((QGraphicsObject *)parent);
    _proxy->setWidget(this);

    _layout = new QGridLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setHorizontalSpacing(5);
    _layout->setVerticalSpacing(0);

    if (name != nullptr) {
        addEntryName(name);
    }

    setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "}"
        "QLabel {"
        "   color: #DCDCDC;"
        "   background-color: transparent;"
        "   font: 14px;"
        "}"
        "QComboBox {"
        "   border: 1px solid gray;"
        "   border-radius: 2px;"
        "   font: 14px;"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "   padding: 3px 30px 1px 3px;"
        "}"
        "QComboBox::drop-down {"
            "subcontrol-origin: padding;"
            "subcontrol-position: top right;"
            "border-top-right-radius: 3px;"
            "border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QListView {"
        "   background-color: #474747;"
        "}"
        "QDoubleSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QDoubleSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QSpinBox {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QSpinBox::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QLineEdit {"
        "   color: #DCDCDC;"
        "   background-color: #242424;"
        "}"
        "QLineEdit::hover {"
        "   color: #EFEFEF;"
        "   background-color: #474747;"
        "}"
        "QCheckBox::indicator {"
        "   width: 14px;"
        "   height: 14px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   border-image: url(:/images/noder/checkbox/unchecked.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:unchecked:hover {"
        "   border-image: url(:/images/noder/checkbox/hover.png) 0 0 0 0 stretch;"
        "}"
        "QCheckBox::indicator:checked {"
        "   border-image: url(:/images/noder/checkbox/checked.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton {"
        "   max-width: 14px;"
        "   max-height: 14px;"
        "}"
        "QPushButton#plus {"
        "   border-image: url(:/images/noder/plus.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#minus {"
        "    border-image: url(:/images/noder/minus.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#plus::hover {"
        "   border-image: url(:/images/noder/plus_hover.png) 0 0 0 0 stretch;"
        "}"
        "QPushButton#minus::hover {"
        "    border-image: url(:/images/noder/minus_hover.png) 0 0 0 0 stretch;"
        "}"
    );
}