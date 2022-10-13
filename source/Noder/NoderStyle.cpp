#include "NoderStyle.hpp"

NoderStyle::NoderStyle()
{
    
}

NoderStyle::NoderStyle(QString filename)
{
    loadTheme(filename);
}

void NoderStyle::loadTheme(QString filename)
{
    QFile file(":/styles/" + filename);
    QByteArray byteArray;
    QJsonDocument doc;
    QJsonObject topobj;
    QJsonObject obj;
    QString color;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file" << filename;
        return;
    }

    byteArray = file.readAll();
    doc = QJsonDocument::fromJson(byteArray);

    topobj = doc.object()["Noder"].toObject();

    setBackgroundCol(topobj["BackgroundCol"].toString());
    setGridCol(topobj["GridCol"].toString());
}

NoderStyle::~NoderStyle()
{
    
}