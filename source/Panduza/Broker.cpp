#include "Broker.hpp"

Broker::Broker(QString addr, unsigned int port)
    :_addr(addr),
    _port(port),
    _enabled(false)
{
    _client = new QMqttClient();
    fillClientProperties();
    connect(_client, &QMqttClient::connected, this, &Broker::connectionEstablished);
    connectToBroker();
}

Broker::Broker()
    : Broker("localhost", 1883)
{

}

void Broker::connectToBroker(void)
{
    _client->connectToHost();
    showBrokerInfos();
}

void Broker::disconnect(void)
{
    if (_client->state() != QMqttClient::Disconnected)
        _client->disconnectFromHost();
}

void Broker::refreshConnection(void)
{
    if (_enabled) {
        disconnect();
        fillClientProperties();
        connectToBroker();
    }
    else {
        disconnect();
    }
}

void Broker::fillClientProperties(void)
{
    _client->setHostname(_addr);
    _client->setPort(_port);
}

void Broker::subscribe(const QString &topic)
{
    qDebug() << "Will subscribe to" << topic;
    _client->subscribe(QMqttTopicFilter(topic));
}

void Broker::publish(const QString &topic ,const QByteArray &message)
{
    _client->publish(QMqttTopicName(topic), message);
}

void Broker::connectionEstablished(void)
{
    qDebug() << "Connected!";
    subscribe("pza/+/+/+/info");
    publish("pza", "*");
    connect(_client, &QMqttClient::messageReceived, this, &Broker::msg);
}

void Broker::msg(const QByteArray &message, const QMqttTopicName &topic)
{
    msgReceived(topic, message);
}

void Broker::showBrokerInfos(void)
{
    qDebug() << "Broker infos:";
    qDebug() << "  Addr:    " << _addr;
    qDebug() << "  Port:    " << _port;
}

Broker::~Broker()
{

}