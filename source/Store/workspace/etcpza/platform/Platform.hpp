#pragma once

// Qt
#include <QMap>
#include <QList>
#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>


struct AutoDetectInfo
{
    // Helpers
    typedef QSharedPointer<AutoDetectInfo> ShPtr;

    QString name;
    QString driver;
    QString group;
    QString config;

    QJsonObject toInterfaceJson() const {
        QJsonObject obj;

        obj["name"] = name;
        obj["driver"] = driver;

        return obj;
    }
};


struct DriverInfo
{

    // Helpers
    typedef QSharedPointer<DriverInfo> ShPtr;

    QString name;
    QString version;
    QString templated;
    QString description;
    QList<AutoDetectInfo::ShPtr> autodetected;


    QJsonObject toInterfaceJson() const {
        QJsonObject obj;
        
        obj["name"] = "unamed";
        obj["driver"] = name;


        return obj;
    }

};

/**
 * 
 * 
 */
class Platform : public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief Construct a new Platform object
         */
        Platform() : QObject() {}

        /**
         * @brief Import data from workspace files
         */
        void import();

        /**
         * @brief 
         */
        const QMap<QString, DriverInfo::ShPtr>& driverInfos() const { return mDriverInfos; }

    signals:

        void updated();

    private:

        QMap<QString, DriverInfo::ShPtr> mDriverInfos;

};

