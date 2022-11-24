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
 * @brief Widget that allow the user to configure the server address and credentials
 * @addtogroup StoreBarModule
 * 
 * Most of the configuration of the layout is performed in the constructor
 * 
 * An internal member _localConnectionInfo manage data locally used by the widget.
 * Methods importFromStore and exportToStore allow the internal data to be synchronized with the store data.
 */
class TabServer : public QWidget
{
    Q_OBJECT
    
    public:

        /**
         * @brief Construct a new Tab Server object
         * 
         * @param parent @see QWidget
         */
        TabServer(QWidget *parent = nullptr);
        
    private slots:

        /**
         * @brief Apply the configuration set by the user to the store
         */
        void exportToStore();

        /**
         * @brief Set the local data from store active configuration and update the widget
         */
        void importFromStore();

        /**
         * @brief Load the server from the library
         * 
         * This function can also create a new server if the index is invalid
         *
         * @param index index of the selected item in the combobox
         */
        void loadServerFromIndex(int index);

        /**
         * @brief Take information from the server library and refresh the combobox
         */
        void refreshServerComboBoxFromServerLibrary();

        /**
         * @brief Configure this widget with local data
         */
        void updateWidgetWithLocalData();

        /**
         * @brief Check if the apply button must be enable or not
         */
        void updateApplyButton(const QString &text = "");

        /**
         * @brief Show or hide the host address row
         */
        void updateHostAddressRow(const QString &text = "");

    private:

        /**
         * @brief Check if the local server info is equal to the store active server info
         * 
         * @return true if equal
         * @return false else
         */
        bool localConnectionInfoEqualToStore()
        {
            return (Store::Get().connection.active == _localConnectionInfo);
        }

        /**
         * @brief Local data object
         *
         * Widget works on this object until the user request the export into the store
         */
        ConnectionInfo _localConnectionInfo;

        // --------------------------------------------------------------------
        // LAYOUT INIT ONLY
        // --------------------------------------------------------------------

        void configureInternalWidgets();

        QWidget* composeGroupCommon();
        QWidget* composeGroupMqtt();
        QWidget* composeGroupAdminSsh();
        QWidget* composeGroupAdminLocal();

        // --------------------------------------------------------------------
        // COMMON GROUP
        // --------------------------------------------------------------------

        QFormLayout mConnectionFormLy;
        QComboBox mConnectionCb;
        PzaLineEdit mNameLe;
        PzaLineEdit* mHostAddressLe;
        QComboBox mHostLocationCb;

        // --------------------------------------------------------------------
        // USER MQTT GROUP
        // --------------------------------------------------------------------

        PzaLineEdit mMqttPortLe;
        // PzaLineEdit mMqttUsernameLe;
        // PzaLineEdit mMqttPasswordLe;

        // --------------------------------------------------------------------
        // ADMIN SSH GROUP
        // --------------------------------------------------------------------

        QWidget* mGroupAdminSsh;
        PzaLineEdit mSshPortLe;
        PzaLineEdit mSshUsernameLe;
        PzaLineEdit mSshPasswordLe;

        // --------------------------------------------------------------------
        // ADMIN LOCAL GROUP
        // --------------------------------------------------------------------

        PzaLineEdit mLocalPasswordLe;
        QWidget* mGroupAdminLocal;

        // --------------------------------------------------------------------
        // FOOTER
        // --------------------------------------------------------------------

        QPushButton mApplyButton;
        QPushButton mDeleteButton;

};

