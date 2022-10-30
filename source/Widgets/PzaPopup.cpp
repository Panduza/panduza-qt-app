#include "PzaPopup.hpp"

PzaPopup::PzaPopup(QWidget *unique, QWidget *parent)
    : PzaMenu(parent)
{
    if (unique) {
        QWidgetAction *action = new QWidgetAction(this);

        action->setDefaultWidget(unique);
        addAction(action);
    }
}

PzaPopup::PzaPopup(const QString &name, QWidget *unique, QWidget *parent)
    : PzaPopup(unique, parent)
{
    QAction *title = new QAction(name);
    title->setEnabled(false);
    insertAction(actions().at(0), title);
}

void PzaPopup::setValidator(void)
{
    QWidgetAction *action = new QWidgetAction(this);
    PzaPushButton *ok = new PzaPushButton(this);

    ok->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    ok->setText("Ok");

    connect(ok, &PzaPushButton::clicked, this, [&]() {
        close();
        validated();
    });

    action->setDefaultWidget(ok);
    addAction(action);
}