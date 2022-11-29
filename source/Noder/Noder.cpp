#include <Noder.hpp>
#include <Pin.hpp>
#include "NoderFrame.hpp"

Noder::Noder()
{
    Frame = new NoderFrame();

    _mapPlugFiles[NodeProperty::PlugType::Array] = initPlugType(NodeProperty::PlugType::Array);
    _mapPlugFiles[NodeProperty::PlugType::Value] = initPlugType(NodeProperty::PlugType::Value);
    _mapPlugFiles[NodeProperty::PlugType::Exec] = initPlugType(NodeProperty::PlugType::Exec);

    _plugExecNcIcon = _mapPlugFiles[NodeProperty::PlugType::Exec].plugNc.data.toUtf8();
    _plugExecCIcon = _mapPlugFiles[NodeProperty::PlugType::Exec].plugC.data.toUtf8();

    ForEachVarType([&](const NoderVarProps::Type type) {
        _mapPlugValueCIcon[type] = LoadCValue(type);
        _mapPlugValueNcIcon[type] = LoadNcValue(type);
        _mapPlugArrayCIcon[type] = LoadCArray(type);
        _mapPlugArrayNcIcon[type] = LoadNcArray(type);
    });
}

QString &Noder::PinTypeName(const PinProperty::Type type)
{
    return PinTypeMap()[type];
}

PinProperty::Type Noder::PinTypeFromName(const QString &name)
{
    for (const auto& [key, value] : PinTypeMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::PinDirName(const PinProperty::Direction type)
{
    return PinDirMap()[type];
}

PinProperty::Direction Noder::PinDirFromName(const QString &name)
{
    for (const auto& [key, value] : PinDirMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::VarTypeName(const NoderVarProps::Type type)
{
    return VarTypeMap()[type].name;
}

QColor &Noder::VarTypeColor(const NoderVarProps::Type type)
{
    return VarTypeMap()[type].color;
}

bool Noder::VarHasSubType(const NoderVarProps::Type type)
{
    return VarTypeMap()[type].hasSubType;
}

const s_varProperties &Noder::VarTypeProperties(const NoderVarProps::Type type)
{
    return VarTypeMap()[type];
}

NoderVarProps::Type Noder::VarTypeFromName(const QString &name)
{
    for (const auto& [key, value] : VarTypeMap())
        if (value.name == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::VarContainerName(const NoderVarProps::Container ctn)
{
    return VarContainerMap()[ctn];
}

NoderVarProps::Container Noder::VarContainerFromName(const QString &name)
{
    for (const auto& [key, value] : VarContainerMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::NodeTypeName(const NodeProperty::Type type)
{
    return NodeTypeMap()[type];
}

NodeProperty::Type Noder::NodeTypeFromName(const QString &name)
{
    for (const auto& [key, value] : NodeTypeMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

const std::vector<QString> &Noder::EnumValues(const QString &name)
{
    return EnumMap()[name];
}

void Noder::ForEachEnum(const std::function<void(const QString &name, const std::vector<QString> &list)> &f)
{
    for (auto const &var : EnumMap()) {
        f(var.first, var.second);
    }
}

void Noder::ForEachEnumName(const std::function<void(const QString &name)> &f)
{
    for (auto const &var : EnumMap()) {
        f(var.first);
    }
}

void Noder::ForEachEnumValues(const QString &name, const std::function<void(const QString &name)> &f)
{
    for (auto const &var : EnumMap()[name]) {
        f(var);
    }
}

void Noder::ForEachVarContainer(const std::function<void(const NoderVarProps::Container ctn)> &f)
{
    for (auto const &var : VarContainerMap()) {
        f(var.first);
    }
}

void Noder::ForEachVarType(const std::function<void(const NoderVarProps::Type type)> &f)
{
    for (auto const &var : VarTypeMap()) {
        f(var.first);
    }
}

void Noder::ForEachPinType(const std::function<void(const PinProperty::Type type)> &f)
{
    for (auto const &var : PinTypeMap()) {
        f(var.first);
    }
}

void Noder::ForEachPinDirection(const std::function<void(const PinProperty::Direction direction)> &f)
{
    for (auto const &var : PinDirMap()) {
        f(var.first);
    }
}

bool Noder::IsTypeCompatible(NoderVarProps::Type type1, NoderVarProps::Type type2)
{
    std::vector<NoderVarProps::Type> &map = VarTypeMap()[type1].compatibleMap;

    if (type1 == type2)
        return true;

    return PzaUtils::IsInVector<NoderVarProps::Type>(map, type2);
}

struct NodeProperty::plugIcon Noder::initPlugType(NodeProperty::PlugType type)
{
    struct NodeProperty::plugIcon s;
    struct NodeProperty::plugIconData dataNc;
    struct NodeProperty::plugIconData dataC;

    switch (type) {
        case NodeProperty::PlugType::Array:
            dataC.filename = ":/images/noder/plug_array.svg";
            dataNc.filename = ":/images/noder/plug_array.svg";
            break;
        case NodeProperty::PlugType::Exec:
            dataC.filename = ":/images/noder/plug_exec_c.svg";
            dataNc.filename = ":/images/noder/plug_exec_nc.svg";
            break;
        case NodeProperty::PlugType::Value:
            dataC.filename = ":/images/noder/plug_value_c.svg";
            dataNc.filename = ":/images/noder/plug_value_nc.svg";
            break;
    }

    auto loadPlugIcon = [](const QString &filename) -> QString {
        QFile file(filename);

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "Could not open" << filename;
            return nullptr;
        }
        QTextStream in(&file);
        QString text = in.readAll();
        file.close();
        return text;
    };

    dataC.data = loadPlugIcon(dataC.filename);
    dataNc.data = loadPlugIcon(dataNc.filename);

    s.plugC = dataC;
    s.plugNc = dataNc;
    
    return s;
}

QByteArray Noder::FillPlugMap(const NodeProperty::PlugType &plugType, const NoderVarProps::Type &type, bool linked)
{
    QString xml;
    QColor color;

    color = VarTypeColor(type);
    if (linked)
        xml = _mapPlugFiles[plugType].plugC.data;
    else
        xml = _mapPlugFiles[plugType].plugNc.data;
    xml.replace("fill:#ffffff", "fill:" + color.name());
    return xml.toUtf8();
}

QByteArray Noder::LoadCValue(const NoderVarProps::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Value, type, 1);
}

QByteArray Noder::LoadNcValue(const NoderVarProps::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Value, type, 0);
}

QByteArray Noder::LoadCArray(const NoderVarProps::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Array, type, 1);
}

QByteArray Noder::LoadNcArray(const NoderVarProps::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Array, type, 0);
}

const QByteArray &Noder::PlugValue(const NoderVarProps::Type type, bool linked)
{
    if (linked)
        return _mapPlugValueCIcon[type];
    else
        return _mapPlugValueNcIcon[type];
}

const QByteArray &Noder::PlugArray(const NoderVarProps::Type type, bool linked)
{
    if (linked)
        return _mapPlugArrayCIcon[type];
    else
        return _mapPlugArrayNcIcon[type];
}

const QByteArray &Noder::PlugExec(bool linked)
{
    if (linked)
        return _plugExecCIcon;
    else
        return _plugExecNcIcon;
}