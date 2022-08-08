#ifndef BROKER_HPP
#define BROKER_HPP

#include <QMqttClient>

class Broker : public QObject
{
    Q_OBJECT
    
    public:
        Broker(QString addr, unsigned int port);
        Broker();
        ~Broker();

        QString addr() const {return _addr;}
        void setAddr(QString addr) {_addr = addr;}
        unsigned int port() const {return _port;}
        void setPort(unsigned int port) {_port = port;}
        void connectToBroker(void);
        void disconnect(void);
        void refreshConnection(void);
        void setEnable(bool state) {_enabled = state;}
        bool enabled() const {return _enabled;}
        void showBrokerInfos(void);
        void subscribe(const QString &new_topic);
        void publish(const QString &topic ,const QByteArray &message);
    
    private:
        bool _enabled;
        QString _addr;
        unsigned int _port;
        QMqttClient *_client;

        void fillClientProperties(void);

    public slots:
        void connectionEstablished(void);
        void msg(const QByteArray &message, const QMqttTopicName &topic);

    signals:
        void msgReceived(const QMqttTopicName &topic, const QByteArray &message);
};

#endif