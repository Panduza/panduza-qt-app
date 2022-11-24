#include "IeBroker.hpp"


#include <QFormLayout>
#include <PzaLabel.hpp>

// ============================================================================
// 
IeBroker::IeBroker(const CfgBroker::ShPtr& data, QWidget *parent)
    : QWidget(parent)
    , _data(data)
{    
    // Sub layout with the form
    auto l0 = new QFormLayout(this);
    l0->addRow(new PzaLabel("Addr"), &_addrLe);
    l0->addRow(new PzaLabel("Port"), &_portLe);

    // 
    _addrLe.setText(data->addr());
    _portLe.setText(data->port());

    // _addrLe
    connect(&_addrLe, &QLineEdit::textChanged, [this](const QString &text){ _data->setAddr(text); });
    connect(&_portLe, &QLineEdit::textChanged, [this](const QString &text){ _data->setPort(text); });
}

