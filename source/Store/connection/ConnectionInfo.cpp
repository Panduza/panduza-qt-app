// Panduza
#include <ConnectionInfo.hpp>

// Qt
#include <QJsonObject>

// ============================================================================
//
void ConnectionInfo::copyFrom(const ConnectionInfo& info)
{
    mId = info.mId;

    mName = info.mName;
    mHostAddress = info.mHostAddress;
    mHostLocation = info.mHostLocation;

    mMqttPort = info.mMqttPort;

    mSshPort = info.mSshPort;
    mSshUsername = info.mSshUsername;
    mSshPassword = info.mSshPassword;

    mLocalPassword = info.mLocalPassword;

    emit updated(*this);
}

// ============================================================================
//
void ConnectionInfo::exportToJson(QJsonObject& obj)
{
    obj["id"] = mId;
    
    obj["name"] = mName;
    obj["host_address"] = mHostAddress;
    obj["host_location"] = static_cast<int>(mHostLocation);

    obj["mqtt_port"] = mMqttPort;

    obj["ssh_port"] = mSshPort;
    obj["ssh_username"] = mSshUsername;
}

// ============================================================================
//
void ConnectionInfo::importFromJson(const QJsonObject& obj)
{
    mId = obj["id"].toInt();
    mName = obj["name"].toString();
    mHostAddress = obj["host_address"].toString();
    mHostLocation = static_cast<HostLocation>(obj["host_location"].toInt());

    mMqttPort = obj["mqtt_port"].toInt();

    mSshPort = obj["ssh_port"].toInt();
    mSshUsername = obj["ssh_username"].toString();
}


