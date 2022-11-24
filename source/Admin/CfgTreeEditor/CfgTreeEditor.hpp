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
#include <QErrorMessage>

// Panduza
#include <Store/Store.hpp>

#include <CfgTreeTreeModel.hpp>
#include <PlatformTreeModel.hpp>

#include <PzaAdminButton.hpp>

#include "ItemEditor/ItemEditorView.hpp"


#include <Utils/QYaml.hpp>

#include <fstream>

/**
 * \brief Widget that manage the editor of the panduza tree
 *
 * The editor takes the CfgTree model of the current workspace
 * 
 * The editor is composed of 3 aeras:
 * 
 * - The tree view of the cfg tree current configuration
 * - The tree view of the platform capabilities
 * - An area to edit the current selected item
 * 
 */
class CfgTreeEditor : public QWidget
{

    public:

        /**
         * @brief Construct a new Object
         */
        CfgTreeEditor(QWidget *parent = nullptr);

    private slots:

        /**
         * @brief Display the context menu for the CfgTree tree view
         */
        void showCfgTreeTreeContextMenu(const QPoint & point);

        /**
         * @brief Display on the editor widget the cfg tree item selected
         * 
         * @param index : index in the cfgtree model
         */
        void showCfgTreeItem(const QModelIndex & index);

    private:

        /**
         * @brief Splitter to organise the widget
         */
        QSplitter _splitter;

        /**
         * @brief TreeView for the config tree
         */
        QTreeView _cfgTreeView;

        /**
         * @brief TreeView for the platform tree
         */
        QTreeView _cfgPlatformView;

        /**
         * @brief QAbstractItemModel for the CfgTree, capatible with QTreeView
         */
        CfgTreeTreeModel _cfgTreeModel;

        /**
         * @brief QAbstractItemModel for the PlatformTree, capatible with QTreeView
         */
        PlatformTreeModel _platformTreeModel;

        /**
         * @brief Central item editor
         */
        ItemEditorView _ItemEditorView;



        QPushButton _saveCfgTreeBt;
        PzaAdminButton _exportCfgTreeBt;
        PzaAdminButton _scanInterfacesBt;



        QErrorMessage _errorDialog;
};

