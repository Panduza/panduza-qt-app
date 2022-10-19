#include "PzaMoreLess.hpp"

PzaMoreLess::PzaMoreLess(const QString &name, PzaStyle::Stretch stretch, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);
    _more = new QPushButton(this);
    _less = new QPushButton(this);
    _name = new PzaLabel(name, this);

    connect(_more, &QPushButton::clicked, this, [&](){more();});
    connect(_less, &QPushButton::clicked, this, [&](){less();});

    _name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _more->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _less->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _more->setObjectName("more");
    _less->setObjectName("less");

    _layout->setSpacing(7);
    _layout->setContentsMargins(0, 0, 0, 0);

    setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "}"
        "QLabel {"
        "   color: #DCDCDC;"
        "   background-color: transparent;"
        "   font: 14px;"
        "}"
        "QPushButton {"
        "   max-width: 14px;"
        "   max-height: 14px;"
        "}"
        "QPushButton#more {"
            "   border-image: url(:/images/noder/plus.png) 0 0 0 0 stretch;"
            "}"
            "QPushButton#less {"
            "    border-image: url(:/images/noder/minus.png) 0 0 0 0 stretch;"
            "}"
            "QPushButton#more::hover {"
            "   border-image: url(:/images/noder/plus_hover.png) 0 0 0 0 stretch;"
            "}"
            "QPushButton#less::hover {"
            "    border-image: url(:/images/noder/minus_hover.png) 0 0 0 0 stretch;"
            "}"
    );

    if (stretch == PzaStyle::Stretch::Left)
        _layout->addStretch(1);

    _layout->addWidget(_more);
    _layout->addWidget(_less);
    _layout->addWidget(_name);

    if (stretch == PzaStyle::Stretch::Right)
        _layout->addStretch(1);
}