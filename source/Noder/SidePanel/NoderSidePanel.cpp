#include "NoderSidePanel.hpp"
#include "NoderSPVariable.hpp"
#include "NoderSPFunction.hpp"
#include "NoderFrame.hpp"

NoderSP::NoderSP(QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    FunctionArea = new NoderSPFunctionArea(_main);
    VariableArea = new NoderSPVarArea(_main);

    PzaSpoiler *functionSpoiler = new PzaSpoiler("Functions", _main);
    PzaSpoiler *variableSpoiler = new PzaSpoiler("Global Variables", _main);

    functionSpoiler->addWidget(FunctionArea);
    variableSpoiler->addWidget(VariableArea);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFixedWidth(800);

    _layout->addWidget(functionSpoiler);
    _layout->addWidget(variableSpoiler);
    _layout->addStretch(1);

    setWidget(_main);
}

void NoderSP::save(void)
{
    QFile file("/home/agouby/toto.json");

    file.open(QIODevice::ReadWrite | QIODevice::Truncate);

    QJsonDocument doc;
    QJsonObject json;

    json["Functions"] = FunctionArea->save();
    //doc.setObject(FunctionArea->save());

    doc.setObject(json);

    file.write(doc.toJson());
}