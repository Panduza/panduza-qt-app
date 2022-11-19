#pragma once

// Qt
#include <QDir>
#include <QDebug>
#include <QWidget>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>


#include <Store/workspace/etcpza/cfgtree/CfgBroker.hpp>

#include <PzaLineEdit.hpp>

// DriverInfo::ShPtr _ptrDriverInfo;
// AutoDetectInfo::ShPtr _ptrAutoDetectInfo;


/**
 *
 * 
 */
class IeBroker : public QWidget
{
    public:

        /**
         *
         */
        IeBroker(const CfgBroker::ShPtr& data, QWidget *parent = nullptr);

    private:

        PzaLineEdit _addrLe;
        PzaLineEdit _portLe;

        CfgBroker::ShPtr _data;

};

