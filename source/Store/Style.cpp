#include "Style.hpp"

Style::Style()
    : QObject()
{
    loadCssFromFile(":/styles/base");
}

void Style::loadCssFromFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open CSS file" << filename;
        return;
    }
    
    stylesheet = file.readAll();
    updated();
}