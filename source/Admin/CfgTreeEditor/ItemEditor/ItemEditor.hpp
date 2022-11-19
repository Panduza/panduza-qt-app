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


/**
 *
 * 
 */
class ItemEditor : public QWidget
{
    public:

        /**
         *
         */
        ItemEditor(QWidget *parent = nullptr);


        void loadItem(const CfgBroker::ShPtr& broker_item);

    private:
    
        QHBoxLayout _layout;

        QWidget* _internal;

};

