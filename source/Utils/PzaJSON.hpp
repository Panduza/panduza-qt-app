#pragma once

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class PzaJSON
{
    public:
        static QString KeyToString(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<QString>(json, key);
        }
        static int KeyToInt(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<int>(json, key);
        }
        static double KeyToDouble(const QJsonDocument &json, const QString &key)
        {
            return PzaJSON::_keyToValue<double>(json, key);
        }

        static QByteArray FromString(const QString &key, const QString &value)
        {
            return PzaJSON::_fromValue<QString>(key, "\"" + value + "\"");
        }

        static QByteArray FromInt(const QString &key, const int &value)
        {
            return PzaJSON::_fromValue<int>(key, value);
        }

        static QByteArray FromDouble(const QString &key, const double &value)
        {
            return PzaJSON::_fromValue<double>(key, value);
        }

        static QByteArray FromBool(const QString &key, const bool &value)
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
            return QString("{\"%1\" : %2}").arg(key).arg(value).toUtf8();
        }
        PzaJSON() = default;
        ~PzaJSON() = default;
};