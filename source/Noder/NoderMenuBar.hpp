#pragma once

#include <PzaMenuBar.hpp>
#include <PzaMenu.hpp>

class NoderMenuBar : public PzaMenuBar
{
    friend class NoderFrame;
    
    private:
        NoderMenuBar(QWidget *parent = nullptr);

        void addActionToMenu(const QString &name, PzaMenu *menu, const std::function<void(void)> &f);
};