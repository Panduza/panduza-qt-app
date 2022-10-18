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


/**
 *
 */
class TabWorkspace : public QWidget
{
    public:

        /**
         * @brief Construct a new Tab Server object
         * 
         * @param parent @see QWidget
         */
        TabWorkspace(QWidget *parent = nullptr);
    
    private:


};

