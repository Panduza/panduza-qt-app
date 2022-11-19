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



    // _addrLe
    connect(&_addrLe, &QLineEdit::textChanged, [this](const QString &text){ _data->setAddr(text); });



    // setAddr(const QString& v, bool emitSignal = true)
}

