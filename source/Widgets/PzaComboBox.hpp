#pragma once

#include <QAbstractItemView>
#include <QComboBox>

class PzaComboBox : public QComboBox
{
    Q_OBJECT

    public:
        PzaComboBox(QWidget *parent)
            : QComboBox(parent)
        {
            setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            setStyleSheet(
                "QComboBox {"
                "   border: 1px solid gray;"
                "   border-radius: 2px;"
                "   font: 14px;"
                "   color: #DCDCDC;"
                "   background-color: #242424;"
                "   padding: 3px 30px 1px 3px;"
                "}"
                "QComboBox::drop-down {"
                    "subcontrol-origin: padding;"
                    "subcontrol-position: top right;"
                    "border-top-right-radius: 3px;"
                    "border-bottom-right-radius: 3px;"
                "}"
                "QComboBox::hover {"
                "   color: #EFEFEF;"
                "   background-color: #474747;"
                "}"
                "QListView {"
                "   background-color: #474747;"
                "}"
            );
        }
        ~PzaComboBox() = default;

        void showPopup(void) override
        {
            QComboBox::showPopup();
            
            // Dummy trick to bypass Scene warning on sendEvent when view() is called
            if (_init == false) {
                view()->parentWidget()->setStyleSheet("background-color: #474747");
                _init = true;
            }

            clicked();
        }

    private:
        bool _init = false;

    signals:
        void clicked(void);
};