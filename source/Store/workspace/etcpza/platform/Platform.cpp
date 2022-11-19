// Panduza
#include "Platform.hpp"
#include <Store/Store.hpp>

// Qt
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// ============================================================================
//
void Platform::import()
{
    qDebug() << "import !!";


    mDriverInfos.clear();


    // pour chaque json dans platform

    auto path = Store::Get().workspace.platformDirPath();

    // qDebug() << path;


    QDir dir(path);
    QStringList json_filenames = dir.entryList(QStringList() << "*.json", QDir::Files);
    foreach(const auto& json_filename, json_filenames)
    {
        auto json_filepath = path + QDir::separator() + json_filename;
        // qDebug() << json_filepath;

        QString val;
        QFile file;
        file.setFileName(json_filepath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject obj = d.object();

        // qDebug() << obj;


        for( auto entry : obj["drivers"].toArray() )
        {
            // qDebug() << ">> " << entry;
            // qDebug() << ">> " << entry.toObject()["template"].toString();

            DriverInfo::ShPtr driver(new DriverInfo());
            driver->name = entry.toObject()["name"].toString();
            driver->version = entry.toObject()["version"].toString();
            driver->description = entry.toObject()["description"].toString();
            driver->templated = QJsonDocument(entry.toObject()["template"].toObject()).toJson();

            // todo auto here

            mDriverInfos[driver->name] = driver;

        }



    }


    // Alert watchers
    emit updated();
}

