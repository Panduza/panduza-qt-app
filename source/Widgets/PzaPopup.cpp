#include "PzaPopup.hpp"

PzaPopup::PzaPopup(QWidget *parent)
    : PzaMenu(parent)
{

}

PzaPopup::PzaPopup(const QString &name, QWidget *parent)
    : PzaPopup(parent)
{
    addTitle(name);
}

void PzaPopup::addTitle(const QString &name)
{
    QAction *title = new QAction(name, this);
    title->setEnabled(false);
    insertAction(actions().at(0), title);
}

void PzaPopup::addWidget(QWidget *w)
{
    QWidgetAction *action = new QWidgetAction(this);

    action->setDefaultWidget(w);
    if (_validator)
        insertAction(_validator, action);
    else
        addAction(action);
}

void PzaPopup::removeWidget(QWidget *w)
{
    QWidgetAction *item;

    for (auto action: actions()) {
        item = dynamic_cast<QWidgetAction *>(action);
        if (item && item->defaultWidget() == w) {
            removeAction(item);
        }
    }
}

void PzaPopup::setValidator(void)
{
    _validator = new QWidgetAction(this);
    PzaPushButton *ok = new PzaPushButton(this);

    ok->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    ok->setText("Ok");

    connect(ok, &PzaPushButton::clicked, this, [&]() {
        close();
        validated();
    });

    _validator->setDefaultWidget(ok);
    addAction(_validator);
}