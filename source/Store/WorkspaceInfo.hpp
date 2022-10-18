#pragma once

// Qt
#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QSettings>


/**
 *
 */
class WorkspaceInfo : public QObject
{
    Q_OBJECT

    public:

        /**
        * @brief Construct a new WorkspaceInfo
        * 
        * @param parent @see QObject
        */
        WorkspaceInfo(QObject* parent = nullptr)
            : QObject(parent)
        {

            QSettings settings;
            mPath = settings.value("workspace/path").toString();

        }

        const QString& path() const { return mPath; }
        void setPath(const QString& path) {

            mPath = path;
            // qDebug() << ">> " << mPath;

            QSettings settings;
            settings.setValue("workspace/path", mPath);

            emit updateddd();
        }


        // bool localPlatformProjetIsEmpty()
        // bool remotePlatformProjetIsEmpty()
        // bool remoteDifferentFromLocal()
        // Import from remote
        // export to remote

    signals:

        /**
        * @brief
        */
        void updateddd();


    private:

        /**
        * @brief Path of the workspace directory
        */
        QString mPath;


};

