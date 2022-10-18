#pragma once

// Qt
#include <QWidget>
#include <QSplitter>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>

// Panduza
#include <Store/Store.hpp>
#include <Store/connection/ConnectionInfo.hpp>
#include <Widgets/PzaLabel.hpp>
#include <Widgets/PzaLineEdit.hpp>


#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>


/**
 *
 */
class TabWorkspace : public QWidget
{
    Q_OBJECT

    public:

        /**
         * @brief Construct a new Tab Server object
         * 
         * @param parent @see QWidget
         */
        TabWorkspace(QWidget *parent = nullptr);
    
    private slots:

        void updateView();

    private:

        QFileSystemModel _model;
        QFileIconProvider _iconProvider;
        QTreeView _treeView;

};

