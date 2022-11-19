#pragma once

// Qt
#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QSettings>

// Panduza
#include "platform/Platform.hpp"
#include "cfgtree/CfgTree.hpp"


// CfgTree

/**
 *
 */
class EtcPza : public QObject
{
    Q_OBJECT

    public:

        /**
        * @brief Construct a new Etc
        * 
        * @param parent @see QObject
        */
        EtcPza(QObject* parent = nullptr)
            : QObject(parent)
        {

        }


        CfgTree tree;

        Platform platform;



};

