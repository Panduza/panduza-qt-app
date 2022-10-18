#pragma once

// Qt
#include <QObject>
#include <QSharedPointer>

/**
 * @brief Represent the host location
 */
enum HostLocation { HostLocationLocal = 0, HostLocationRemote = 1 };

/**
 * @brief Contain the information about a server
 *
 */
class ConnectionInfo : public QObject
{
    Q_OBJECT

    public:

        // Helpers
        typedef QSharedPointer<ConnectionInfo> ShPtr;

        // Default Ports
        static constexpr int SshDefaultPort = 22;
        static constexpr int MqttDefaultPort = 1883;

        /**
         * @brief Construct a new Server Info object
         * 
         * @param parent @see QObject
         */
        ConnectionInfo(int id = -1, QObject* parent = nullptr)
            : QObject(parent)
            , mId(id)
            , mHostLocation(HostLocationLocal)
            , mMqttPort(MqttDefaultPort)
            , mSshPort(SshDefaultPort)
        { }

        /**
         * @brief Import value from the other info inside this instance and trigger the updated signal 
         * 
         * @param info the object from which the info must be copied
         */
        void copyFrom(const ConnectionInfo& info);

        // BASIC GETTERS
        int id() const { return mId; }
        const QString& name() const { return mName; }
        HostLocation hostLocation() const { return mHostLocation; }
        const QString& hostAddress() const { return mHostAddress; }
        int mqttPort() const { return mMqttPort; }
        int sshPort() const { return mSshPort; }
        const QString& sshUsername() const { return mSshUsername; }
        const QString& sshPassword() const { return mSshPassword; }
        const QString& localPassword() const { return mLocalPassword; }

        // BASIC SETTERS
        void setId(int v) { mId = v; emit updated(*this); }
        void setName(const QString& v) { mName = v; emit updated(*this); }
        void setHostLocation(HostLocation v) { mHostLocation = v; emit updated(*this); }
        void setHostAddress(const QString& v) { mHostAddress = v; emit updated(*this); }
        void setSshPort(int v) { mSshPort = v; emit updated(*this); }
        void setSshUsername(const QString& v) { mSshUsername = v; emit updated(*this); }
        void setSshPassword(const QString& v) { mSshPassword = v; emit updated(*this); }
        void setLocalPassword(const QString& v) { mLocalPassword = v; emit updated(*this); }

        // EXPORT / IMPORT
        void exportToJson(QJsonObject& obj);
        void importFromJson(const QJsonObject& obj);

    signals:

        /**
         * @brief Triggered when the server has been updated
         */
        void updated(const ConnectionInfo& info);

    private:

        /**
         * @brief Internal ID for the library management
         */
        int mId;

        /**
         * @brief Friendly name of the server
         */
        QString mName;

        /**
         * @brief Host location (local or remote)
         */
        HostLocation mHostLocation;

        /**
         * @brief Server host address
         * 
         * It could be an IPV4 or a name to be resolved
         */
        QString mHostAddress;

        // --------------------------------------------------------------------
        // MQTT
        // --------------------------------------------------------------------

        /**
         * @brief Port for the mqtt connection (default @see MqttDefaultPort)
         */
        int mMqttPort;

        // --------------------------------------------------------------------
        // SSH
        // --------------------------------------------------------------------

        /**
         * @brief Port for the mqtt connection (default @see SshDefaultPort)
         */
        int mSshPort;

        /**
         * @brief SSH username
         */
        QString mSshUsername;

        /**
         * @brief SSH password
         */
        QString mSshPassword;

        // --------------------------------------------------------------------
        // LOCAL
        // --------------------------------------------------------------------

        QString mLocalPassword;

};

/**
 * @brief Check if 2 objects are equal
 * 
 * @return true if the two object are the same
 * @return false else
 */
inline bool operator==(const ConnectionInfo& a, const ConnectionInfo& b)
{
    return (
        a.name()            == b.name()             &&
        a.hostLocation()    == b.hostLocation()     &&
        a.hostAddress()     == b.hostAddress()      &&
        a.mqttPort()        == b.mqttPort()         &&
        a.sshPort()         == b.sshPort()          &&
        a.sshUsername()     == b.sshUsername()      &&
        a.sshPassword()     == b.sshPassword()      &&
        a.localPassword()   == b.localPassword()
        );
}


