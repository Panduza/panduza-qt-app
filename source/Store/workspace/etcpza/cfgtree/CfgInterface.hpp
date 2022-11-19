#pragma once

// Qt
#include <QObject>
#include <QJsonObject>

/**
 * \brief Configuration data for a panduza interface
 * 
 * The name and the driver are common to all interface configuration they must be present
 * The remaining of the configuration depend of the driver
 */
class CfgInterface : public QObject
{
    Q_OBJECT

    public:
        // Helpers
        typedef QSharedPointer<CfgInterface> ShPtr;

        /**
         * @brief Construct a new Cfg Interface object
         * 
         * @param parent 
         */
        CfgInterface(QObject* parent = nullptr)
            : QObject(parent)
        {
            _obj["name"] = "unamed";
            _obj["driver"] = "undefined";
        }

        /**
         * @brief Copy Constructor
         * 
         * @param from 
         * @param parent 
         */
        CfgInterface(const CfgInterface& from, QObject* parent = nullptr)
            : QObject(parent)
        {
            _obj = from._obj;
        }

        /**
         * @brief The name of the interface
         */
        QString name() const
        {
            auto found = _obj.constFind("name");
            if(found == _obj.constEnd()) return QString("unamed");
            return (*found).toString();
        }

        /**
         * @brief Simple name setter
         */
        void setName(const QString& name)
        {
            _obj["name"] = name;
            emit updated();
        }

        /**
         * @brief The name of the driver
         */
        QString driver() const
        {
            auto found = _obj.constFind("driver");
            if(found == _obj.constEnd()) return QString("unamed");
            return (*found).toString();
        }

        /**
         * @brief Simple driver setter
         */
        void setDriver(const QString& v)
        {
            _obj["driver"] = v;
            emit updated();
        }

        /**
         * @brief Internal data object getter
         */
        const QJsonObject& jsonObject() const { return _obj; }

        /**
         * @brief Internal data object setter
         */
        void setJsonObject(const QJsonObject& obj, bool emitSignal = true) { _obj = obj; if(emitSignal) emit updated(); }

        /**
         * @brief Check if 2 objects are equal
         * 
         * @return true if the two object are the same
         * @return false else
         */
        bool operator==(const CfgInterface& other)
        {
            return (_obj == other._obj);
        }

        bool operator!=(const CfgInterface& other)
        {
            return !(*this==other);
        }

    signals:

        void updated();

    private:

        /**
         * @brief All the data are stored as json object
         */
        QJsonObject _obj;

};

Q_DECLARE_METATYPE(CfgInterface::ShPtr)
