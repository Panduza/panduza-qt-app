// Panduza
#include "TabServer.hpp"
#include <Store/Store.hpp>

// Qt
#include <QDebug>
#include <QGroupBox>

// ============================================================================
//
TabServer::TabServer(QWidget *parent)
    : QWidget(parent)
    , mHostAddressLe(nullptr)
    , mApplyButton(tr("Apply"))
    , mDeleteButton(tr("Delete Connection"))
{
    // COMPOSE LAYOUT
    mGroupAdminSsh = composeGroupAdminSsh();
    mGroupAdminLocal = composeGroupAdminLocal();
    auto l0 = new QVBoxLayout(this);
    l0->addWidget(composeGroupCommon());
    l0->addWidget(composeGroupMqtt());
    l0->addWidget(mGroupAdminSsh);
    l0->addWidget(mGroupAdminLocal);
    l0->addStretch(1);
    l0->addWidget(&mApplyButton);
    l0->addWidget(&mDeleteButton);

    // Bring widgets to life
    configureInternalWidgets();

    // Trigger a first import
    importFromStore();
}

// ============================================================================
//
void TabServer::exportToStore()
{
    // log
    // qDebug() << "pok";

    // Copy local data into the store
    Store::Get().connection.active.copyFrom(mLocalConnectionInfo);

    // Check again if the button must be enabled
    updateApplyButton();
}

// ============================================================================
//
void TabServer::importFromStore()
{
    // Copy from the store
    mLocalConnectionInfo.copyFrom(Store::Get().connection.active);

    //
    updateWidgetWithLocalData();
}

// ============================================================================
//
void TabServer::updateApplyButton(const QString &text)
{
    Q_UNUSED(text);
    if(localConnectionInfoEqualToStore())
    {
        // Data are the same no need to export
        mApplyButton.setEnabled(false);
    }
    else
    {
        // Enable the button to export the local data
        mApplyButton.setEnabled(true);
    }
}

// ============================================================================
//
void TabServer::loadServerFromIndex(int index)
{
    // Log
    // qDebug() << "loadServerFromIndex(" << index << " / " << mConnectionCb.count() << ")";

    // Last element => need to create a new connection
    if( index == mConnectionCb.count()-1 )
    {
        auto srv = Store::Get().connection.library.createNewConnection();
        Store::Get().connection.active.copyFrom(*srv);
    }
    // Load the correct connection
    else if ( index >= 0 && index < mConnectionCb.count()-1 )
    {
        auto srv = Store::Get().connection.library.connection(index);
        Store::Get().connection.active.copyFrom(*srv);
    }
}

// ============================================================================
//
void TabServer::refreshServerComboBoxFromServerLibrary()
{
    // Log
    // qDebug() << "refreshServerComboBoxFromServerLibrary";


    mConnectionCb.disconnect(this);
    mConnectionCb.clear();

    auto items = Store::Get().connection.library.names();
    items << tr("Select to create New Server");

    mConnectionCb.addItems(items);
    connect(&mConnectionCb, &QComboBox::currentIndexChanged, this, &TabServer::loadServerFromIndex);
}

// ============================================================================
//
void TabServer::updateWidgetWithLocalData()
{
    // Log
    // qDebug() << "updateWidgetWithLocalData";

    // Combobox to select the server
    // Disconnect to avoid infinite loop
    mConnectionCb.disconnect(this);
    mConnectionCb.setCurrentIndex(mLocalConnectionInfo.id());
    connect(&mConnectionCb, &QComboBox::currentIndexChanged, this, &TabServer::loadServerFromIndex);

    // 
    mNameLe.setText( mLocalConnectionInfo.name() );

    // 
    mMqttPortLe.setText( QString::number(mLocalConnectionInfo.mqttPort()) );

    // 
    mSshPortLe.setText( QString::number(mLocalConnectionInfo.sshPort()) );
    mSshUsernameLe.setText( mLocalConnectionInfo.sshUsername() );
    mSshPasswordLe.setText( mLocalConnectionInfo.sshPassword() );

    // 
    if(mLocalConnectionInfo.hostLocation() == HostLocationLocal)
    {
        mGroupAdminSsh->hide();
        mGroupAdminLocal->show();
        if(mHostAddressLe) mHostAddressLe->hide();
    }
    else if(mLocalConnectionInfo.hostLocation() == HostLocationRemote)
    {
        mGroupAdminSsh->show();
        mGroupAdminLocal->hide();
        if(mHostAddressLe) mHostAddressLe->show();
    }


    updateHostAddressRow();

    // Check again if the button must be enabled
    updateApplyButton();
}

// ============================================================================
//
void TabServer::updateHostAddressRow(const QString &text)
{
    Q_UNUSED(text);
    if(mLocalConnectionInfo.hostLocation() == HostLocationLocal && mHostAddressLe)
    {
        mConnectionFormLy.removeRow(2);
        // After this point the widget mHostAddressLe is deleted
        // So set it to null
        mHostAddressLe = nullptr;
    }
    else if(mLocalConnectionInfo.hostLocation() == HostLocationRemote && !mHostAddressLe)
    {
        mHostAddressLe = new PzaLineEdit();
        mConnectionFormLy.insertRow(2, new PzaLabel("Address"), mHostAddressLe);
        mHostAddressLe->setText( mLocalConnectionInfo.hostAddress() );
    }
}

// ============================================================================
//
void TabServer::configureInternalWidgets()
{
    // SERVER CONF
    // Prepare the combobox to select the server info
    refreshServerComboBoxFromServerLibrary();
    connect(&mConnectionCb, &QComboBox::currentIndexChanged, this, &TabServer::loadServerFromIndex);

    // === NAME
    connect(&mNameLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setName(text); });
    connect(&mNameLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === HOST
    connect(mHostAddressLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setHostAddress(text); });
    connect(mHostAddressLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === ADMIN MODE
    // Configure the combobox to choose between ssh or local admin mode
    mHostLocationCb.addItems({ tr("This Computer"), tr("Remote Server") });
    connect(&mHostLocationCb, &QComboBox::currentIndexChanged, [this](int index) {
        switch(index)
        {
            case 0:
                mLocalConnectionInfo.setHostLocation(HostLocationLocal);
                break;
            case 1:
                mLocalConnectionInfo.setHostLocation(HostLocationRemote);
                break;
        }
        updateWidgetWithLocalData();
    });

    // === SSH PORT
    mSshPortLe.setValidator( new QIntValidator(0, 50000, this) );
    connect(&mSshPortLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setSshPort(text.toInt()); });
    connect(&mSshPortLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === SSH USERNAME
    connect(&mSshUsernameLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setSshUsername(text); });
    connect(&mSshUsernameLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === SSH PASSWORD
    mSshPasswordLe.setEchoMode(QLineEdit::Password);
    connect(&mSshPasswordLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setSshPassword(text); });
    connect(&mSshPasswordLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === LOCAL PASSWORD
    mLocalPasswordLe.setEchoMode(QLineEdit::Password);
    connect(&mLocalPasswordLe, &QLineEdit::textChanged, [this](const QString &text){ mLocalConnectionInfo.setLocalPassword(text); });
    connect(&mLocalPasswordLe, &QLineEdit::textChanged, this, &TabServer::updateApplyButton);

    // === APPLY
    // At first there is nothing to save
    mApplyButton.setEnabled(false);
    connect(&mApplyButton, &QPushButton::clicked, this, &TabServer::exportToStore);

    // === DELETE
    connect(&mDeleteButton, &QPushButton::clicked, [this](){
        auto srv = Store::Get().connection.library.deleteConnection(
            Store::Get().connection.active.id()
        );
        Store::Get().connection.active.copyFrom(*srv);
    });
        
    // 
    // CONNECTIONS WITH THE STORE
    // 
    // When a new server is created into the store, the combobox must be updated
    connect(&Store::Get().connection.library, &ConnectionLibrary::updated, this, &TabServer::refreshServerComboBoxFromServerLibrary);
    // When the active server change it must be imported inside this view
    connect(&Store::Get().connection.active, &ConnectionInfo::updated, this, &TabServer::importFromStore);
}

// ============================================================================
//
QWidget* TabServer::composeGroupCommon()
{
    // Sub layout with the form
    mConnectionFormLy.addRow(new PzaLabel("Name"), &mNameLe);
    mConnectionFormLy.addRow(new PzaLabel("Host"), &mHostLocationCb);

    // Main layout
    auto l1 = new QVBoxLayout();
    l1->addWidget(&mConnectionCb);
    l1->addLayout(&mConnectionFormLy);
    
    // Main layout
    auto group = new QGroupBox(tr("Connection"));
    group->setLayout(l1);
    return group;
    
}

// ============================================================================
//
QWidget* TabServer::composeGroupMqtt()
{
    // Sub layout with the form
    auto l0 = new QFormLayout();
    l0->addRow(new PzaLabel("Port"), &mMqttPortLe);

    // Main layout
    auto group = new QGroupBox(tr("User (MQTT)"));
    group->setLayout(l0);
    return group;
}

// ============================================================================
//
QWidget* TabServer::composeGroupAdminSsh()
{
    // Sub layout with the form
    auto l0 = new QFormLayout();
    l0->addRow(new PzaLabel("Port"), &mSshPortLe);
    l0->addRow(new PzaLabel("Username"), &mSshUsernameLe);
    l0->addRow(new PzaLabel("Password"), &mSshPasswordLe);

    // Main layout
    auto group = new QGroupBox(tr("Admin (SSH)"));
    group->setLayout(l0);
    return group;
}

// ============================================================================
//
QWidget* TabServer::composeGroupAdminLocal()
{
    // Sub layout with the form
    auto l0 = new QFormLayout();
    l0->addRow(new PzaLabel("Password"), &mLocalPasswordLe);

    // Main layout
    auto group = new QGroupBox(tr("Admin (LOCAL)"));
    group->setLayout(l0);
    return group;
}
