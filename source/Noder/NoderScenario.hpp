#pragma once

#include "NoderView.hpp"

class NoderScenario : public NoderView
{
    Q_OBJECT

    public:
        NoderScenario(QWidget *parent = nullptr);
        ~NoderScenario() = default;

        void setName(void) {_name = _panelName->text(); nameChanged();}
        const QString &name(void) const {return _name;}
        PzaLineEdit *panelName(void) const {return _panelName;}

    private:
        QString _name;
        PzaLineEdit *_panelName;
    
    signals:
        void nameChanged(void);
};