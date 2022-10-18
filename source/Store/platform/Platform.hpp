#pragma once

// Qt
#include <QList>
#include <QObject>

#include "tree/Tree.hpp"

/**
 * 
 * ConfigTree
 * ConfigData
 * Drivers
 * Autodetected
 * 
 */
class Platform : public QObject
{
    Q_OBJECT

    public:

        Platform() : QObject() {}

    private:

        Tree mTree;
};

