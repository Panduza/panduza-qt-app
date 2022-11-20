#include <PzaMenu.hpp>

PzaMenu::PzaMenu(QWidget *parent)
    : QMenu(parent)
{

}

PzaMenu::PzaMenu(const QString &title, QWidget *parent)
    : PzaMenu(parent)
{
    setTitle(title);
}