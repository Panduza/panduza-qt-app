// Panduza
#include "TabWorkspace.hpp"
#include <Store/Store.hpp>

// Qt
#include <QDebug>
#include <QFileDialog>

#include <QJsonObject>
#include <QJsonDocument>



#include <Store/Store.hpp>

// ============================================================================
//
TabWorkspace::TabWorkspace(QWidget *parent)
    : QWidget(parent)
{

    auto rootPath = Store::Get().workspace.path();

    qDebug() << rootPath;

    _model.setIconProvider(&_iconProvider);
    _model.setRootPath("");
    // if (parser.isSet(dontUseCustomDirectoryIconsOption))
    //     _model.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    // if (parser.isSet(dontWatchOption))
    //     _model.setOption(QFileSystemModel::DontWatchForChanges);


    _treeView.setModel(&_model);
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = _model.index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid())
            _treeView.setRootIndex(rootIndex);
    }

    // Demonstrating look and feel features
    _treeView.setAnimated(false);
    _treeView.setIndentation(20);
    _treeView.setSortingEnabled(true);
    const QSize availableSize = _treeView.screen()->availableGeometry().size();
    _treeView.resize(availableSize / 2);
    _treeView.setColumnWidth(0, _treeView.width() / 3);

    // Make it flickable on touchscreens
    QScroller::grabGesture(&_treeView, QScroller::TouchGesture);

    // _treeView.setWindowTitle(QObject::tr("Dir View"));
    // _treeView.show();





    auto change_workspace_button = new QPushButton("change workspace");
    connect(change_workspace_button, &QPushButton::clicked, [this](){
        auto filepath = QFileDialog::getExistingDirectory(this, "Select Workspace");
        if(!filepath.isEmpty())
        {
            qDebug() << ">> " << filepath;
            Store::Get().workspace.setPath(filepath);
        }
    });


    auto lay = new QVBoxLayout(this);
    lay->addWidget(&_treeView);
    lay->addWidget(change_workspace_button);


    connect( &Store::Get().workspace, &WorkspaceInfo::updateddd, this, &TabWorkspace::updateView );

    qDebug() << "init ok";
}

// ============================================================================
//
void TabWorkspace::updateView()
{
    qDebug() << "up";

    QString rootPath( Store::Get().workspace.path() );
    qDebug() << "path ::: " << rootPath;




    _treeView.setModel(&_model);
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = _model.index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid())
            _treeView.setRootIndex(rootIndex);
    }

}

