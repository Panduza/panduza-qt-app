#ifndef NODEPROPERTYDROPDOWN_HPP
#define NODEPROPERTYDROPDOWN_HPP

#include <PzaComboBox.hpp>

#include <NodeProperty.hpp>

class NodePropertyDropdown : public NodeProperty
{
    public:
        NodePropertyDropdown(QString name, Node *parent);
        NodePropertyDropdown(Node *parent);
        ~NodePropertyDropdown();

        void setDropdownList(const QStringList &list) {_combo->addItems(list);}
        QString const value() {return _combo->currentText();}

    private:
        PzaComboBox *_combo;

    public slots:
        void activate(void);
};

#endif