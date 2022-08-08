#ifndef PZAJSON_HPP
#define PZAJSON_HPP

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class PzaJSON
{
    public:
        static QString keyToString(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<QString>(json, key);
        }
        static int keyToInt(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<int>(json, key);
        }
        static double keyToDouble(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<double>(json, key);
        }

        static QByteArray fromString(const QString &key, const QString &value)
        {
            return PzaJSON::_fromValue<QString>(key, "\"" + value + "\"");
        }

        static QByteArray fromInt(const QString &key, const int &value)
        {
            return PzaJSON::_fromValue<int>(key, value);
        }

        static QByteArray fromDouble(const QString &key, const double &value)
        {
            return PzaJSON::_fromValue<double>(key, value);
        }

        static QByteArray fromBool(const QString &key, const bool &value)
        {
            return PzaJSON::_fromValue<QString>(key, (value) ? "true" : "false");
        }

    private:

        template<typename R>
        static R _keyToValue(const QJsonDocument &json, const QString &key)
        {
            R ret = 0;
            QJsonObject obj = json.object();

            if (obj.contains(key))
                ret = obj[key].toVariant().value<R>();

            return ret;
        }

        template<typename R>
        static QByteArray _fromValue(const QString &key, const R &value)
        {
            QString mdr = QString("{\"%1\" : %2}").arg(key).arg(value);

            return mdr.toUtf8();
        }
        PzaJSON() = default;
        ~PzaJSON() = default;
};

#endif