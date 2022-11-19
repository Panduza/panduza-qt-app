#pragma once

// Qt
#include <QList>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

// std
#include <atomic>

class CfgTreeTreeItem;



class CfgTreeTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit CfgTreeTreeModel(QObject *parent = nullptr);
        ~CfgTreeTreeModel();

        /**
         * @brief FROM QAbstractItemModel
         */
        QVariant data(const QModelIndex &index, int role) const override;

        /**
         * @brief FROM QAbstractItemModel
         */
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

        /**
         * @brief FROM QAbstractItemModel
         */
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        
        /**
         * @brief FROM QAbstractItemModel
         */
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;


        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        /**
         * @brief FROM QAbstractItemModel
         */
        QMimeData *mimeData(const QModelIndexList &indexes) const;

        /**
         * @brief FROM QAbstractItemModel
         */
        bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;

        /**
         * @brief FROM QAbstractItemModel
         */
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

        /**
         * @brief FROM QAbstractItemModel
         */
        bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        Qt::DropActions supportedDropActions() const
        {
            return Qt::CopyAction | Qt::MoveAction;
        }


        /**
         * @brief Prevent external model signal from affecting this model
         */
        void lockExternalSync();

        /**
         * @brief Authorize external model signal from affecting this model
         */
        void unlockExternalSync();


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
        CfgTreeTreeItem* _rootItem;



        std::atomic_int _lockCount;

};

