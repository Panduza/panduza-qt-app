#pragma once

// Qt
#include <QList>
#include <QObject>

// Panduza
#include "CfgBroker.hpp"

/**
 * \defgroup ModuleCfgTree Config Tree
 * 
 * \brief Manage the data model of the tree.json
 *
 * This object can load the tree.json from the workspace or save into it
 * 
 * This object stores 2 models:
 * 
 * - the current model: this is the model currenlty used and modified by the user.
 * - the saved model: this is the model corresponding to the files saved in the workspace.
 *
 * The save model is used to known if the tree has been modified
 *
 */
class CfgTree : public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief Construct a new Cfg Tree object
         */
        CfgTree(QObject* parent = nullptr);

        //
        // LOADING
        // 

        /**
         * @brief Load this object with data from the workspace
         */
        bool loadFromWorkspace();

        /**
         * @brief Load this object with data from a json string
         */
        void loadFromString(const QByteArray& data);

        /**
         * @brief Load this object with data from a json object
         */
        void loadFromJson(const QJsonObject& obj);

        //
        // Saving
        // 

        /**
         * @brief Save the tree into the workspace
         */
        bool saveToWorkspace();

        /**
         * @brief Save this into a json object
         */
        void saveToJson(QJsonObject& obj);

        //
        // Brokers list management
        // 

        /**
         * @brief Request the creation of a new broker at the end of the list
         */
        CfgBroker::ShPtr insertBroker(bool emitSignal = true);

        /**
         * @brief Getter for the broker list
         */
        const QList<CfgBroker::ShPtr>& brokers() const { return _brokers; }

        /**
         * @brief Check if the given name already exist among brokers
         * 
         * @param name 
         * @return true 
         * @return false 
         */
        bool isBrockerNameExist(const QString& name);

        //
        // Saved model management
        // 

        /**
         * @brief Check if the current data has been modified and require to be saved
         * 
         * @return true if modified
         * @return false else
         */
        bool needToBeSaved();

        /**
         * @brief Take the current model and copy it on the saved one for them to be equal
         */
        void copyCurrentOnSavedModel();

        //
        // Error management
        // 

        /**
         * @brief Simple getter of the last error
         */
        const QString& errString() const { return _errString; }

    signals:

        /**
         * @brief Emitted when an internal element of the tree has been modified
         */
        void updated();

        /**
         * @brief Emitted when the tree is updated from an external cause
         * 
         * For example causes can be:
         * 
         * - Loading from workspace file
         *
         */
        void updatedFromOutside();

    private:

        /**
         * @brief Machine name
         */
        QString _machine;

        /**
         * @brief Managed Brokers
         */
        QList<CfgBroker::ShPtr> _brokers;

        /**
         * @brief Machine name saved in the workspace
         */
        QString _machineSaved;

        /**
         * @brief Brokers data saved in the workspace
         */
        QList<CfgBroker::ShPtr> _brokersSaved;

        /**
         * @brief Description of the last error
         */
        QString _errString;

};

