#include "Io.hpp"

Io::Io(Group *group, const QString &name)
    : Interface(group, name)
{
    _attsMap["direction"] = std::bind(&Io::parseDirection, this, std::placeholders::_1);
    _attsMap["value"] = std::bind(&Io::parseValue, this, std::placeholders::_1);
}

void Io::sendDirection(const QString &direction)
{
    sendMsg("direction", PzaJSON::fromString("direction", direction));
}

void Io::sendValue(const int value)
{
    sendMsg("value", PzaJSON::fromInt("value", value));
}