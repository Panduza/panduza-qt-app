#pragma once

// Qt
#include <QList>
#include <QObject>


// Panduza
#include "BrokerDeclaration.hpp"

/**
 * 
 */
class Tree : public QObject
{
    Q_OBJECT

    public:

        Tree() : QObject() {}

    public slots:

        void importFromSsh();

        void importFromJson(const QByteArray& data);

    signals:

        void updated();

    private:

        QList<BrokerDeclaration> mBrokers;
};

