// Panduza
#include "Tree.hpp"
#include "Store/Store.hpp"

// Qt
#include <QJsonObject>
#include <QJsonDocument>



// ============================================================================
//
void Tree::importFromSsh()
{
    // if ( Store::Get().connection.ssh.isRemoteFileExist("/etc/panduza/tree.json") )
    // {
    //     qDebug() << "pok";

    //     QByteArray content;
    //     // Store::Get().connection.ssh.getFile("/etc/panduza/tree.json", content);

    //     importFromJson(content);
    // }
}

// ============================================================================
//
void Tree::importFromJson(const QByteArray& data)
{

    QJsonDocument doc = QJsonDocument::fromJson(data);
    auto obj = doc.object();

    qDebug() << obj;

    qDebug() << "\n";

    auto brokers = obj["brokers"].toObject() ;
    qDebug() << brokers;

    for( auto broker : brokers )
    {
        qDebug() << "broker\n";
        qDebug() << broker;
    }

    
    

}



