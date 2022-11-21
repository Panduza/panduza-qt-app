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
#include <Store/workspace/etcpza/cfgtree/CfgInterface.hpp>


/**
 *
 * 
 */
class ItemEditorView : public QWidget
{
    public:

        /**
         *
         */
        ItemEditorView(QWidget *parent = nullptr);


        void loadItem(const CfgBroker::ShPtr& broker_item);
        void loadItem(const CfgInterface::ShPtr& interface_item);

    private:
    
        QHBoxLayout _layout;

        QWidget* _internal;

};

