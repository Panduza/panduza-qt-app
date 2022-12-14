#pragma once

#include <QWidgetAction>
#include <QVBoxLayout>

#include <PzaPushButton.hpp>
#include <PzaMenu.hpp>
#include <PzaLabel.hpp>

class PzaPopup : public QMenu
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
        QVBoxLayout *_layout;
        PzaPushButton *_validator = nullptr;

    signals:
        void validated(void);
};