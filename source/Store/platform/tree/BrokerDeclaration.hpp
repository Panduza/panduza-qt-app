#pragma once

// Qt
#include <QList>
#include <QObject>


// Panduza
#include "InterfaceDeclaration.hpp"


/**
 * 
 */
class BrokerDeclaration : public QObject
{
    Q_OBJECT


    public:



    private:

        QList<InterfaceDeclaration> mInterfaces;
};

