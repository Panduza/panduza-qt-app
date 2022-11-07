#pragma once

#include <PzaMenuBar.hpp>
#include <PzaMenu.hpp>

class NoderMenuBar : public PzaMenuBar
{
    public:
        NoderMenuBar(QWidget *parent = nullptr);

    private:
        void addActionToMenu(const QString &name, PzaMenu *menu, const std::function<void(void)> &f);
};