#include <PzaMenu.hpp>

PzaMenu::PzaMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(
        "QMenu {"
        "   background-color: #121212;"
        "   color: #DDDDDD;"
        "   margin: 3px;"
        "}"
        "QMenu::separator {"
        "   height: 1px;"
        "   margin: 5px 0px 5px 0px;"
        "   background: #563223;"
        "}"
        "QMenu::item {"
        "   padding: 2px 25px 2px 20px;"
        "   font: 14px;"
        "}"
        "QMenu::item:selected {"
        "   color: white;"
        "   border-radius: 6px;"
        "   background: #7F3112;"
        "}"
        "QLabel {"
        "   background-color: #121212;"
        "   color: #AAAAAA;"
        "   margin: 3px;"
        "   font: 13px;"
        "}"
        "QLineEdit {"
        "   background-color: #121212;"
        "   color: #DDDDDD;"
        "   border: 1px solid #563223;"
        "   border-radius: 3px;"
        "   margin: 5px 5px 5px 5px;"
        "}"
    );
}

PzaMenu::PzaMenu(const QString &title, QWidget *parent)
    : PzaMenu(parent)
{
    setTitle(title);
}