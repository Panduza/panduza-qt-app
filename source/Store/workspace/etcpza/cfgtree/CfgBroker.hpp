#pragma once

// Qt
#include <QList>
#include <QObject>
#include <QSharedPointer>

// Panduza
#include "CfgInterface.hpp"

/**
 * \brief This object stores information to explain the platform how to connect to a broker
 * 
 */
class CfgBroker : public QObject
{
    Q_OBJECT

    public:

        // Default values
        static constexpr const char* default_broker_name = "new_broker";

        // Helpers
        typedef QSharedPointer<CfgBroker> ShPtr;

        /**
         * @brief Construct a new Cfg Broker object
         */
        CfgBroker();

        /**
         * @brief Copy constructor
         * 
         * @param from 
         * @param parent 
         */
        CfgBroker(const CfgBroker& from, QObject* parent = nullptr);

        // 
        // NAME
        // 

        /**
         * @brief Simple name getter
         */
        const QString& name() const { return _name;}

        /**
         * @brief Simple name setter
         */
        void setName(const QString& v) { _name = v; emit updated(); }

        // 
        // ADDR
        // 

        const QString& addr() const { return _addr; }
        void setAddr(const QString& v, bool emitSignal = true) { _addr = v; if(emitSignal) emit updated(); }

        const QString& port() const { return _port; }
        void setPort(const QString& v, bool emitSignal = true) { _port = v; if(emitSignal) emit updated(); }

        const QList<CfgInterface::ShPtr>& interfaces() const { return _interfaces; }
        
        /**
         *
         */
        void createNewInterace();

        /**
         * @brief Load this object with data from a json object
         */
        void loadFromJson(const QJsonObject& obj);

        /**
         * @brief Save this into a json object
         */
        void saveToJson(QJsonObject& obj);

        /**
         * @brief Request the creation of a new interface at the end of the list
         */
        CfgInterface::ShPtr insertInterface(int row = 0, bool emitSignal = true);

        //
        // COMPARAISON
        // 

        /**
         * @brief Check if 2 objects are equal
         * 
         * @return true if the two object are the same
         * @return false else
         */
        bool operator==(const CfgBroker& other);

        bool operator!=(const CfgBroker& other)
        {
            return !(*this==other);
        }

    signals:

        /**
         * @brief Emitted when an element of this object or an element of a sub object is modified
         */
        void updated();

    private:

        /**
         * @brief Name of the broker
         */
        QString _name;

        /**
         * @brief Address or Hostname of the broker
         */
        QString _addr;

        /**
         * @brief Mqtt Port used by this broker
         */
        QString _port;

        /**
         * @brief Interfaces attached to this broker
         * 
         */
        QList<CfgInterface::ShPtr> _interfaces;
        
};

Q_DECLARE_METATYPE(CfgBroker::ShPtr)
