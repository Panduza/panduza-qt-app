#include "PzaPopup.hpp"

PzaPopup::PzaPopup(QWidget *parent)
    : QMenu()
{
    _layout = new QVBoxLayout(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PzaPopup::PzaPopup(const QString &name, QWidget *parent)
    : PzaPopup(parent)
{
    addTitle(name);
}

void PzaPopup::addTitle(const QString &name)
{
   _layout->insertWidget(0, new PzaLabel(name));
}

void PzaPopup::addWidget(QWidget *w)
{
    if (_validator)
        _layout->insertWidget(_layout->indexOf(_validator), w);
    else
        _layout->addWidget(w);

    adjustSize();
    update();
}

void PzaPopup::removeWidget(QWidget *w)
{
    _layout->removeWidget(w);
}

void PzaPopup::setValidator(void)
{
    _validator = new PzaPushButton(this);

    _validator->setText("Ok");

    connect(_validator, &PzaPushButton::clicked, this, [&]() {
        close();
        validated();
    });

    _layout->addWidget(_validator);
}