// Panduza
#include "CfgTreeEditor.hpp"
#include <Store/Store.hpp>

#include <CfgTreeTreeItem.hpp>

// Qt
#include <QMenu>
#include <QLabel>
#include <QPixmap>
#include <QHeaderView>

// ============================================================================
// 
CfgTreeEditor::CfgTreeEditor(QWidget *parent)
    : QWidget(parent)
    , _saveCfgTreeBt(QIcon(":/image/save"), "Save")
    , _exportCfgTreeBt("Export\nTree")
    , _scanInterfacesBt("Scan\nInterfaces")
    , _errorDialog(this)
{
    // Save button configuration
    // _saveCfgTreeBt.setFlat(true);
    _saveCfgTreeBt.setIconSize(QSize(40,40));







    // auto save_button = new QPushButton("Save Tree");
    // ll->addWidget(save_button);
    // connect(save_button, &QPushButton::clicked, [this]() {
    //     Store::Get().workspace.etc.tree.saveToWorkspace();
    // });


    // auto init_button = new QPushButton("Initialize Project");
    // ll->addWidget(init_button);
    // connect(init_button, &QPushButton::clicked, [this]() {


    //     auto platform_path = Store::Get().workspace.path() + QDir::separator() + "etcpza";
    //     QDir().mkpath(platform_path);
        
    //     auto data_path = platform_path + QDir::separator() + "data";
    //     QDir().mkpath(data_path);


    //     // 
    //     QFile file(":/rsc/files/mosquitto.conf");
    //     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    //         return;
    //     QByteArray content = file.readAll();
    //     file.close();


    //     auto mosquitto_conf_path = data_path + QDir::separator() + "mosquitto.conf";
    //     QFile ofile(mosquitto_conf_path);
    //     if (!ofile.open(QIODevice::WriteOnly | QIODevice::Text))
    //         return;
    //     ofile.write(content);
    //     ofile.close();
        

    //     // 
    //     // docker-compose.yml


    //     auto panduza_py_platform_node = [](){
    //         YAML::Node n;
            
    //         n["image"] = "ghcr.io/panduza/panduza-py-platform:latest";
    //         n["privileged"] = true;
    //         n["network_mode"] = "host";
    //         n["volumes"] = QVector<QString>() << ".:/etc/panduza" << "/run/udev:/run/udev:ro";

    //         return n;
    //     };

    //     auto panduza_cxx_platform_node = [](){
    //         YAML::Node n;
            
    //         n["image"] = "ghcr.io/panduza/panduza-cxx-platform:latest";
    //         n["privileged"] = true;
    //         n["network_mode"] = "host";
    //         n["environment"] = QVector<QString>() << "AUTODETECT" ;
    //         n["volumes"] = QVector<QString>() << ".:/etc/panduza" ;

    //         return n;
    //     };

    //     YAML::Node dcy;
    //     dcy["version"] = "3";

    //     YAML::Node services;

    //     services["panduza-py-platform"] = panduza_py_platform_node();
    //     services["panduza-cxx-platform"] = panduza_cxx_platform_node();

    //     dcy["services"] = services;
        
        


    //     auto dcy_path = platform_path + QDir::separator() + "docker-compose.yml";
        
    //     qDebug() << "gen file";
    //     std::ofstream ofile2(dcy_path.toStdString());
    //     ofile2 << dcy;
    //     ofile2 << "\n";
    //     ofile2.close();


    //     qDebug() << "remplace";
            
    //     QByteArray fileData_tmp;
    //     QFile file_tmp(dcy_path);
    //     if( !file_tmp.open(QIODevice::ReadWrite) )
    //     {
    //         qDebug() << "fail to open";
    //     } // open for read and write
    //     fileData_tmp = file_tmp.readAll(); // read all the data into the byte array
    //     QString text_tmp(fileData_tmp); // add to text string for easy string replace
    //     qDebug() << ">>> " << text_tmp;
    //     text_tmp.replace(QString("version: 3"), QString("version: '3'")); // replace text in string
    //     file_tmp.seek(0); // go to the beginning of the file
    //     file_tmp.write(text_tmp.toUtf8()); // write the new text back to the file
    //     file_tmp.close(); // close the file handle.
    


    //     Store::Get().connection.admin.copyWorkspacePlatformToRemote();


    // });





    // Disable buttons
    _saveCfgTreeBt.setEnabled(false);
    _exportCfgTreeBt.setEnabled(false);

    // _cfgTreeView
    _cfgTreeView.setModel(&_cfgTreeModel);
    _cfgTreeView.setDragDropMode(QAbstractItemView::DragDrop);
    _cfgTreeView.setDragEnabled(true);
    _cfgTreeView.setAcceptDrops(true);
    _cfgTreeView.setDropIndicatorShown(true);
    _cfgTreeView.setContextMenuPolicy(Qt::CustomContextMenu);
    connect(&_cfgTreeView, &QTreeView::customContextMenuRequested, this, &CfgTreeEditor::showCfgTreeTreeContextMenu);
    connect(&_cfgTreeView, &QTreeView::clicked, this, &CfgTreeEditor::showCfgTreeItem);

    // _cfgPlatformView
    _cfgPlatformView.setModel(&_platformTreeModel);
    _cfgPlatformView.setDragEnabled(true);
    _cfgPlatformView.setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Left header
    QPixmap image(":/img_tree");
    QLabel *imageLabel = new QLabel();    
    imageLabel->setPixmap(image.scaled(64, 64, Qt::KeepAspectRatio));
    imageLabel->setObjectName("blue-back");
    QLabel *textLabel = new QLabel("Interface\nTree");
    textLabel->setObjectName("blue-back");
    auto hl = new QHBoxLayout();
    hl->addWidget(imageLabel);
    hl->addWidget(textLabel, 1);

    // Middle header with buttons
    auto hm = new QHBoxLayout();
    hm->addWidget(&_saveCfgTreeBt, 1);
    hm->addWidget(&_exportCfgTreeBt, 1);
    hm->addWidget(&_scanInterfacesBt, 1);

    // Left bar
    auto left_bar = new QWidget();
    auto left_bar_layout = new QVBoxLayout(left_bar);
    left_bar_layout->addLayout(hl);
    left_bar_layout->addWidget(&_cfgTreeView);
    left_bar_layout->setSpacing(0);
    left_bar_layout->setContentsMargins(0, 0, 0, 0);
    
    // Right bar
    auto right_bar = new QWidget();
    auto right_bar_layout = new QVBoxLayout(right_bar);
    right_bar_layout->addWidget(&_cfgPlatformView);
    right_bar_layout->setSpacing(0);
    right_bar_layout->setContentsMargins(0, 0, 0, 0);

    // Middle bar
    auto middle_bar = new QWidget();
    auto middle_bar_layout = new QVBoxLayout(middle_bar);
    middle_bar_layout->addLayout(hm);
    middle_bar_layout->addWidget(&_ItemEditorView);

    // Splitter
    _splitter.addWidget(left_bar);
    _splitter.setStretchFactor(1, 0);
    _splitter.addWidget(middle_bar);
    _splitter.setStretchFactor(1, 1);
    _splitter.addWidget(right_bar);
    _splitter.setStretchFactor(2, 0);

    // Main layout
    auto main_layout = new QGridLayout(this);
    main_layout->addWidget(&_splitter);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // When the user click on the save button
    connect(&_saveCfgTreeBt, &QPushButton::clicked, [this]() {
        // Save action on the store model will cause an update event
        // Tree model must ignore this signal to aoid a regeneration
        _cfgTreeModel.lockExternalSync();
        if( ! Store::Get().workspace.etc.tree.saveToWorkspace() )
        {
            _errorDialog.showMessage(Store::Get().workspace.etc.tree.errString());
        }
        _cfgTreeModel.unlockExternalSync();
    });

    // When the user click on the scan interface button
    connect(_scanInterfacesBt.internalComponent(), &QPushButton::clicked, [this]() {

        _scanInterfacesBt.setEnabled(false);
        Store::Get().connection.admin.execAutodetectPlatform([this](){
            qDebug() << "end !!!";
            _scanInterfacesBt.setEnabled(true);
        });

    });

    // Adapt the state of the save button when the tree is updated
    auto saveBtResfreshState = [this]() {
        // qDebug() << "saveBtResfreshState";
        if(Store::Get().workspace.etc.tree.needToBeSaved())
        {
            _saveCfgTreeBt.setEnabled(true);
        }
        else
        {
            _saveCfgTreeBt.setEnabled(false);
        }
    };
    saveBtResfreshState();
    connect(&Store::Get().workspace.etc.tree, &CfgTree::updated, saveBtResfreshState);
}

// ============================================================================
// 
void CfgTreeEditor::showCfgTreeTreeContextMenu(const QPoint & point)
{
    QModelIndex index = _cfgTreeView.indexAt(point);


    if (!index.isValid()) {

        QMenu *menu=new QMenu(this);

        // New Broker Action
        auto act_new_broker = new QAction(tr("New Broker"), menu);
        connect(act_new_broker, &QAction::triggered, [this, index]() {
            _cfgTreeModel.insertRows( 0, 1, QModelIndex() );
        });
        menu->addAction(act_new_broker);
        
        menu->popup(_cfgTreeView.viewport()->mapToGlobal(point));

    }

    if (index.isValid()) {

        QMenu *menu=new QMenu(this);

        auto *item = static_cast<CfgTreeTreeItem*>(index.internalPointer());

        switch (item->type())
        {
        case CfgTreeTreeItem::TypeBroker:
        {
            auto act_new_iface = new QAction("New Interface", menu);
            connect(act_new_iface, &QAction::triggered, [this, index, item](){
                
                // item->ptrBroker()->createNewInterace();

                _cfgTreeModel.insertRows( index.row() , 1, index );

            });
            menu->addAction(act_new_iface);
            break;   
        }
        
        default:
            break;
        };
        

        menu->popup(_cfgTreeView.viewport()->mapToGlobal(point));

            
        // contextMenu->exec(ui->tree_cfgTreeView.viewport()->mapToGlobal(point));

        // auto item = dynamic_cast<PzaTreeTreeItem*>(model->itemFromIndex(index));


    }

}

// ============================================================================
// 
void CfgTreeEditor::showCfgTreeItem(const QModelIndex& index)
{
    auto *item = static_cast<CfgTreeTreeItem*>(index.internalPointer());

    switch(item->type())
    {
        case CfgTreeTreeItem::TypeBroker:
            _ItemEditorView.loadItem( item->ptrBroker() );
            break;

        case CfgTreeTreeItem::TypeInterface:
            break;

        default:
            break;
    }
}



