#pragma once

// Qt
#include <QDir>
#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QSettings>

// 
#include "etcpza/EtcPza.hpp"

/**
 *
 */
class Workspace : public QObject
{
    Q_OBJECT

    public:

        // Only the Store can create a workspace object
        friend class Store;

        /**
         * @brief 
         * 
         */
        EtcPza etc;


        const QString& path() const { return mPath; }
        void setPath(const QString& path) {

            mPath = path;
            // qDebug() << ">> " << mPath;

            QSettings settings;
            settings.setValue("workspace/path", mPath);

            // Defines workspace paths
            mRootDirPath = mPath;
            mEtcPzaDirPath = mRootDirPath + QDir::separator() + "etcpza";
            mPlatformDirPath = mEtcPzaDirPath + QDir::separator() + "platform";

            _cfgTreeJsonFilePath = mEtcPzaDirPath + QDir::separator() + "tree.json";

            emit updated();
        }


        // 
        // HELPER GETTER FOR VARIOUS PATH OF THE WORKSPACE
        // 

        const QString& rootDirPath() const { return mRootDirPath;}
        const QString& etcPzaDirPath() const { return mEtcPzaDirPath; }
        const QString& platformDirPath() const { return mPlatformDirPath; }


        const QString& cfgTreeJsonFilePath() const { return _cfgTreeJsonFilePath; }

    signals:

        /**
        * @brief
        */
        void updated();


    private:

        /**
        * @brief Construct a new Workspace
        * 
        * @param parent @see QObject
        */
        Workspace(QObject* parent = nullptr)
            : QObject(parent)
        {

            QSettings settings;
            setPath(settings.value("workspace/path").toString());

        }
        
        /**
        * @brief Path of the workspace directory
        */
        QString mPath;


        QString mRootDirPath;
        QString mEtcPzaDirPath;
        QString _cfgTreeJsonFilePath;
        QString mPlatformDirPath;
        


        

};

