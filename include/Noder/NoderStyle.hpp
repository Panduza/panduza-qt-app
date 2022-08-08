#ifndef NODERSTYLE_HPP
#define NODERSTYLE_HPP

#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class NoderStyle
{
    public:
        NoderStyle();
        NoderStyle(QString filename);
        ~NoderStyle();

        void loadTheme(QString filename);

        QColor backgroundCol(void) const { return _backgroundcol; };
        void setBackgroundCol(QString color) { _backgroundcol = QColor(color); };

        QColor gridCol(void) const { return _gridcol; };
        void setGridCol(QString color) { _gridcol = QColor(color); };

    private:
        QColor _backgroundcol;
        QColor _gridcol;


};

#endif