#pragma once

#include <QWidgetAction>

#include <PzaPushButton.hpp>
#include <PzaMenu.hpp>

class PzaPopup : public PzaMenu
{
    Q_OBJECT

    public:
        PzaPopup(const QString &name, QWidget *unique = nullptr, QWidget *parent = nullptr);
        PzaPopup(QWidget *unique = nullptr, QWidget *parent = nullptr);

        void setValidator(void);

    signals:
        void validated(void);
};