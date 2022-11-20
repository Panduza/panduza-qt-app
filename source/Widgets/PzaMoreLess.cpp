#include "PzaMoreLess.hpp"

PzaMoreLess::PzaMoreLess(QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QHBoxLayout(this);
    _more = new PzaPushButton(this);
    _less = new PzaPushButton(this);

    connect(_more, &PzaPushButton::clicked, this, [&](){more();});
    connect(_less, &PzaPushButton::clicked, this, [&](){less();});

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _more->setObjectName("more");
    _less->setObjectName("less");

    _layout->setSpacing(7);
    _layout->setContentsMargins(0, 0, 0, 0);

    _layout->addWidget(_more);
    _layout->addWidget(_less);
}

PzaMoreLess::PzaMoreLess(const QString &name, QWidget *parent)
    : PzaMoreLess(parent)
{
    _name = new PzaLabel(name, this);
    _layout->addWidget(_name);
}