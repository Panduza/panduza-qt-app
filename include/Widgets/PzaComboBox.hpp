#pragma once

#include <QComboBox>

class PzaComboBox : public QComboBox
{
    Q_OBJECT

    public:
        PzaComboBox(QWidget *parent)
            : QComboBox(parent)
        {
        }
        ~PzaComboBox() = default;

        void showPopup(void) override
        {
            QComboBox::showPopup();

            clicked();
        }

    signals:
        void clicked(void);
};