#pragma once

#include <QTreeWidgetItem>
#include <PzaTreeWidget.hpp>
#include <QMouseEvent>

class PzaTreeWidgetItem : public QTreeWidgetItem
{
    public:
        PzaTreeWidgetItem(QTreeWidget *parent = nullptr);
        PzaTreeWidgetItem(QTreeWidgetItem *parent = nullptr);

        bool isCategory(void) const {return _isCategory;}
        void setCategory(bool state) {_isCategory = state;}

        void setSvgIcon(int index, const QByteArray &data);

    private:
        bool _isCategory = false;
};