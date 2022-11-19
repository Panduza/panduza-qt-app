#pragma once

// Qt
#include <QObject>
#include <QSharedPointer>

/**
 *
 */
class ServerInstallModel : public QObject
{
    Q_OBJECT

    public:

        /**
         * 
         */
        ServerInstallModel(QObject* parent = nullptr)
            : QObject(parent)
            , mInstallSshServer(true)
            // , mId(id)
            // , mHostLocation(HostLocationLocal)
            // , mMqttPort(MqttDefaultPort)
            // , mSshPort(SshDefaultPort)
        { }

        /**
         * @brief Import value from the other info inside this instance and trigger the updated signal 
         * 
         * @param info the object from which the info must be copied
         */
        // void copyFrom(const InstallModel& info);

        // // BASIC GETTERS
        // int id() const { return mId; }
        // const QString& name() const { return mName; }
        // HostLocation hostLocation() const { return mHostLocation; }
        // const QString& hostAddress() const { return mHostAddress; }
        // int mqttPort() const { return mMqttPort; }
        // int sshPort() const { return mSshPort; }
        // const QString& sshUsername() const { return mSshUsername; }
        // const QString& sshPassword() const { return mSshPassword; }
        // const QString& localPassword() const { return mLocalPassword; }

        // // BASIC SETTERS
        // void setId(int v) { mId = v; emit updated(*this); }
        // void setName(const QString& v) { mName = v; emit updated(*this); }
        // void setHostLocation(HostLocation v) { mHostLocation = v; emit updated(*this); }
        // void setHostAddress(const QString& v) { mHostAddress = v; emit updated(*this); }
        // void setSshPort(int v) { mSshPort = v; emit updated(*this); }
        // void setSshUsername(const QString& v) { mSshUsername = v; emit updated(*this); }
        // void setSshPassword(const QString& v) { mSshPassword = v; emit updated(*this); }
        // void setLocalPassword(const QString& v) { mLocalPassword = v; emit updated(*this); }

        // // EXPORT / IMPORT
        // void exportToJson(QJsonObject& obj);
        // void importFromJson(const QJsonObject& obj);

    signals:

        /**
         * @brief Triggered when the server has been updated
         */
        // void updated(const InstallModel& info);

    private:

        /**
         * 
         */
        bool mInstallSshServer;


};



