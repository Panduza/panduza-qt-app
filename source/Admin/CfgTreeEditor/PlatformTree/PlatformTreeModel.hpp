#pragma once

// Qt
#include <QList>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


class PlatformTreeItem;


class PlatformTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit PlatformTreeModel(QObject *parent = nullptr);
        ~PlatformTreeModel();

        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column,
                        const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;



        QMimeData* mimeData(const QModelIndexList &indexes) const;

        QStringList mimeTypes() const;
        Qt::DropActions supportedDropActions() const;

    private slots:

        /**
         * @brief Synchronise the model with the store
         */
        void syncWithStore();


    private:


        /**
         * @brief 
         * 
         */
        PlatformTreeItem* _rootItem;


};

