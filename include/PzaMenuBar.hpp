#pragma once

#include <QMenuBar>

class PzaMenuBar : public QMenuBar
{
    public:
        PzaMenuBar()
            : QMenuBar()
        {
            setStyleSheet(
            "QMenuBar {"
            "   background-color: #121212;"
            "   color: #DDDDDD;"
            "   spacing: 3px;"
            "}"
            "QMenuBar::item {"
            "   padding: 5px 5px 5px 5px;"
            "   font: 14px;"
            "}"
            "QMenuBar::item:selected {"
            "   color: white;"
            "   border-radius: 6px;"
            "   background: #7F3112;"
            "}"
            );
        }
        ~PzaMenuBar() = default;
};