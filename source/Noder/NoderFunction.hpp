#pragma once

#include "NoderView.hpp"

class NoderFunction : public NoderView
{
    Q_OBJECT
    
    public:
        NoderFunction(QWidget *parent = nullptr);
        ~NoderFunction() = default;

        void setName(void) {_name = _panelName->text(); nameChanged();}
        const QString &name(void) const {return _name;}
        PzaLineEdit *panelName(void) const {return _panelName;}

    private:
        QString _name;
        PzaLineEdit *_panelName;
    
    signals:
        void nameChanged(void);
};