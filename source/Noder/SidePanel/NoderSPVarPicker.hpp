#pragma once

#include <QWidgetAction>
#include <QHBoxLayout>

#include <PzaWidget.hpp>
#include <PzaTreeWidget.hpp>
#include <PzaTreeWidgetItem.hpp>
#include <PzaComboBox.hpp>
#include <PzaToolButton.hpp>
#include "NoderSidePanel.hpp"

class NoderSPVarPicker : public PzaWidget
{
    Q_OBJECT

    public:
        NoderSPVarPicker(QWidget *parent = nullptr);

        void setVar(const NoderVarProps &varProps);
        const NoderVarProps &varProps(void) const {return _varProps;}

    private:
        NoderVarProps _varProps;
        PzaTreeWidgetItem *_variableCat;
        PzaTreeWidgetItem *_interfaceCat;
        PzaTreeWidgetItem *_enumCat;
        PzaToolButton *_typeDrop;
        PzaComboBox *_ctnDrop;
        QHBoxLayout *_layout;

        PzaTreeWidgetItem *addItem(const QString &name, PzaTreeWidgetItem *parent = nullptr);
        PzaTreeWidgetItem *addCategory(const QString &name, PzaTreeWidget *parent = nullptr);
        void refreshContainerIco(void);
        void refreshTypeIco(void);
        void configureTree(void);

        void setType(const NoderVarProps::Type type, const QString &subType = "");
        void setContainer(const NoderVarProps::Container ctn);
    
    signals:
        void varChanged(const NoderVarProps &varProps);
};