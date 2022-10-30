#pragma once

#include <QAbstractItemView>
#include <QComboBox>
#include <PzaWidget.hpp>

class PzaComboBox : public QComboBox
{
    Q_OBJECT

    public:
        PzaComboBox(QWidget *parent = nullptr);

        void showPopup(void) override;

    signals:
        void clicked(void);
};