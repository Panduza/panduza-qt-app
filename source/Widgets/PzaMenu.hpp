#pragma once

#include <QMenu>
#include <PzaWidget.hpp>

class PzaMenu : public QMenu
{
    public:
        PzaMenu(const QString &title, QWidget *parent = nullptr);
        PzaMenu(QWidget *parent = nullptr);
};