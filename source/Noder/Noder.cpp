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

    forEachVarType([&](const NoderVar::Type type) {
        _mapPlugValueCIcon[type] = LoadCValue(type);
        _mapPlugValueNcIcon[type] = LoadNcValue(type);
        _mapPlugArrayCIcon[type] = LoadCArray(type);
        _mapPlugArrayNcIcon[type] = LoadNcArray(type);
    });
}

QString &Noder::pinTypeName(const PinProperty::Type type)
{
    return pinTypeMap()[type];
}

PinProperty::Type Noder::pinTypeFromName(const QString &name)
{
    for (const auto& [key, value] : pinTypeMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::pinDirName(const PinProperty::Direction type)
{
    return pinDirMap()[type];
}

PinProperty::Direction Noder::pinDirFromName(const QString &name)
{
    for (const auto& [key, value] : pinDirMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::varTypeName(const NoderVar::Type type)
{
    return varTypeMap()[type].name;
}

QColor &Noder::varTypeColor(const NoderVar::Type type)
{
    return varTypeMap()[type].color;
}

const s_varProperties &Noder::varTypeProperties(const NoderVar::Type type)
{
    return varTypeMap()[type];
}

NoderVar::Type Noder::varTypeFromName(const QString &name)
{
    for (const auto& [key, value] : varTypeMap())
        if (value.name == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::varContainerName(const NoderVar::Container ctn)
{
    return varContainerMap()[ctn];
}

NoderVar::Container Noder::varContainerFromName(const QString &name)
{
    for (const auto& [key, value] : varContainerMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

QString &Noder::nodeTypeName(const NodeProperty::Type type)
{
    return nodeTypeMap()[type];
}

NodeProperty::Type Noder::nodeTypeFromName(const QString &name)
{
    for (const auto& [key, value] : nodeTypeMap())
        if (value == name)
            return key;
    // Should not be possible
    std::abort();
}

const std::vector<QString> &Noder::enumValues(const QString &name)
{
    return enumMap()[name];
}

void Noder::forEachEnum(const std::function<void(const QString &name, const std::vector<QString> &list)> &f)
{
    for (auto const &var : enumMap()) {
        f(var.first, var.second);
    }
}

void Noder::forEachEnumName(const std::function<void(const QString &name)> &f)
{
    for (auto const &var : enumMap()) {
        f(var.first);
    }
}

void Noder::forEachEnumValues(const QString &name, const std::function<void(const QString &name)> &f)
{
    for (auto const &var : enumMap()[name]) {
        f(var);
    }
}

void Noder::forEachVarContainer(const std::function<void(const NoderVar::Container ctn)> &f)
{
    for (auto const &var : varContainerMap()) {
        f(var.first);
    }
}

void Noder::forEachVarType(const std::function<void(const NoderVar::Type type)> &f)
{
    for (auto const &var : varTypeMap()) {
        f(var.first);
    }
}

void Noder::forEachPinType(const std::function<void(const PinProperty::Type type)> &f)
{
    for (auto const &var : pinTypeMap()) {
        f(var.first);
    }
}

void Noder::forEachPinDirection(const std::function<void(const PinProperty::Direction direction)> &f)
{
    for (auto const &var : pinDirMap()) {
        f(var.first);
    }
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

QByteArray Noder::FillPlugMap(const NodeProperty::PlugType &plugType, const NoderVar::Type &type, bool linked)
{
    QString xml;
    QColor color;

    color = varTypeColor(type);
    if (linked)
        xml = _mapPlugFiles[plugType].plugC.data;
    else
        xml = _mapPlugFiles[plugType].plugNc.data;
    xml.replace("fill:#ffffff", "fill:" + color.name());
    return xml.toUtf8();
}

QByteArray Noder::LoadCValue(const NoderVar::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Value, type, 1);
}

QByteArray Noder::LoadNcValue(const NoderVar::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Value, type, 0);
}

QByteArray Noder::LoadCArray(const NoderVar::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Array, type, 1);
}

QByteArray Noder::LoadNcArray(const NoderVar::Type &type)
{
    return FillPlugMap(NodeProperty::PlugType::Array, type, 0);
}

const QByteArray &Noder::PlugValue(const NoderVar::Type type, bool linked)
{
    if (linked)
        return _mapPlugValueCIcon[type];
    else
        return _mapPlugValueNcIcon[type];
}

const QByteArray &Noder::PlugArray(const NoderVar::Type type, bool linked)
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