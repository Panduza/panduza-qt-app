#pragma once

#include <QWidgetAction>
#include <QHBoxLayout>

#include <PzaWidget.hpp>
#include <PzaTreeWidget.hpp>
#include <PzaTreeWidgetItem.hpp>
#include <PzaComboBox.hpp>
#include <PzaToolButton.hpp>
#include "NoderSidePanel.hpp"

class NoderSPVarDrop : public PzaWidget
{
    Q_OBJECT

    public:
        NoderSPVarDrop(QWidget *parent = nullptr);

        void setType(NoderVar::Type type);
        NoderVar::Type type(void) const {return _type;}
        void setContainer(NoderVar::Container ctn);
        NoderVar::Container container(void) const {return _ctn;}

    private:
        PzaTreeWidgetItem *_variableCat;
        PzaTreeWidgetItem *_interfaceCat;
        PzaTreeWidgetItem *_enumCat;
        PzaToolButton *_typeDrop;
        PzaComboBox *_ctnDrop;
        NoderVar::Type _type;
        NoderVar::Container _ctn;
        QHBoxLayout *_layout;

        PzaTreeWidgetItem *addItem(const QString &name, PzaTreeWidgetItem *parent = nullptr);
        PzaTreeWidgetItem *addCategory(const QString &name, PzaTreeWidget *parent = nullptr);
        void refreshContainerIco(void);
        void refreshTypeIco(void);
        void configureTree(void);
    
    signals:
        void varChanged(NoderVar::Container ctn, NoderVar::Type type);
};