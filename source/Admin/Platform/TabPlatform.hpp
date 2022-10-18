#pragma once

// Qt
#include <QDebug>
#include <QWidget>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

// Panduza

#include "PzaTreeWidget/PzaTreeTreeModel.hpp"

/**
 *
 */
class TabPlatform : public QWidget
{
    public:
        TabPlatform(QWidget *parent = nullptr)
            : QWidget(parent)
        {
            


            // _content = new QStackedWidget();
            // _content->addWidget(tabContainer);
            // _content->addWidget(new QLabel("no password !!!!"));


            // model de données workspace
            // model de données remote

            // 1) chargement de tous ces models dans le store
            // 2) alert si différent workspace diff de remote => proposer à l'utilisateur d'importer la config distante/de charger la config workspace
            // 3) ensuite l'app ne travail que sur le workspace et synchronise le remote quand l'utilisateur appuie sur le boutton


            // tab configuration
            // en haut (import from server / load to server     / enable/disbale server)
            // machine name
            // tree view for current tree right side
            // view for autodetected
            // configuration piloter au milieu (zone de text qui change si le driver dispose d'une view specific)


            QString data = "truc \n machin \t lalalala \n chose \n";
            auto model = new PzaTreeTreeModel(data);
            

            auto ll = new QHBoxLayout();
            ll->addWidget(new QPushButton("import"));
            ll->addWidget(new QPushButton("export"));
            

            auto init_button = new QPushButton("Initialize Project");
            ll->addWidget(init_button);
            connect(init_button, &QPushButton::clicked, [this]() {

                // Create the local conf
                // export the conf
                // run the autodetect mode
                // get result

            });


            auto view = new QTreeView();
            view->setModel(model);

            auto view2 = new QTreeView();
            view2->setModel(model);

            auto lay = new QGridLayout(this);
            lay->addWidget(view, 0, 0, 2, 1);
            lay->addWidget(new QTextEdit(), 1, 1);
            lay->addLayout(ll, 0, 1);
            lay->setColumnStretch(1, 1);
            lay->addWidget(view2, 0, 2, 2, 1);
            
            

        }
        

};

