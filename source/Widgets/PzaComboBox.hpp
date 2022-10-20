#pragma once

#include <QAbstractItemView>
#include <QComboBox>
#include <PzaWidget.hpp>

class PzaComboBox : public QComboBox
{
    Q_OBJECT

    public:
        PzaComboBox(QWidget *parent = nullptr);
        ~PzaComboBox() = default;

        void showPopup(void) override;

    private:
        bool _init = false;

    signals:
        void clicked(void);
};