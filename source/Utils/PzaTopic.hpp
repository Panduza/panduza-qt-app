#pragma once

#include <QString>
#include <QStringList>

class PzaTopic
{
    public:
        int parseTopic(const QString &topic)
        {
            QStringList list = topic.split("/");

            if (list.count() <= PosInTopic::Type) {
                qDebug() << "Topic" << topic << "is malformed.";
                return -1;
            }

            _machine = list[PosInTopic::Machine];
            _group = list[PosInTopic::Group];
            _interface = list[PosInTopic::Interface];
            _type = list[PosInTopic::Type];

            if (list.count() > PosInTopic::Property) {
                for (int i = PosInTopic::Property; i < list.count();) {
                    _property += list[i];
                    if (++i != list.count())
                        _property += "/";
                }
            }
            return 0;
        }

        const QString &machine() const {return _machine;}
        const QString &group() const {return _group;}
        const QString &interface() const {return _interface;}
        const QString &type() const {return _type;}
        const QString &property() const {return _property;}

    private:
        enum PosInTopic {
            Pza = 0,
            Machine,
            Group,
            Interface,
            Type,
            Property
        };

        QString _machine;
        QString _group;
        QString _interface;
        QString _type;
        QString _property;
};