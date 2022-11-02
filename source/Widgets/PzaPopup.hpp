#pragma once

#include <QWidgetAction>

#include <PzaPushButton.hpp>
#include <PzaMenu.hpp>

class PzaPopup : public PzaMenu
{
    Q_OBJECT

    public:
        PzaPopup(const QString &name, QWidget *parent = nullptr);
        PzaPopup(QWidget *parent = nullptr);

        void addWidget(QWidget *w);
        void removeWidget(QWidget *w);

        void addTitle(const QString &name);
        void setValidator(void);

    private:
        QWidgetAction *_validator = nullptr;

    signals:
        void validated(void);
};