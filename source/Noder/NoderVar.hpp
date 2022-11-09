#pragma once

#pragma once

#include "Noder.hpp"

class NoderVarBase
{
public:
    NoderVar::Type type(void) const { return _type; }

protected:
    NoderVar::Type _type;
};

class NoderVarBool : public NoderVarBase
{
public:
    NoderVarBool()
    {
        _type = NoderVar::Type::Bool;
    }

    void setValue(bool value) { _value = value; }
    bool value(void) const { return _value; }

private:
    bool _value;
};

class NoderVarInt : public NoderVarBase
{
public:
    NoderVarInt()
    {
        _type = NoderVar::Type::Int;
    }

    void setValue(int value) { _value = value; }
    int value(void) const { return _value; }

private:
    int _value;
};

class NoderVarFloat : public NoderVarBase
{
public:
    NoderVarFloat()
    {
        _type = NoderVar::Type::Float;
    }

    void setValue(double value) { _value = value; }
    double value(void) const { return _value; }

private:
    double _value;
};

class NoderVarString : public NoderVarBase
{
public:
    NoderVarString()
    {
        _type = NoderVar::Type::String;
    }

    void setValue(const QString &value) { _value = value; }
    const QString &value(void) const { return _value; }

private:
    QString _value;
};

class NoderVarEnum : public NoderVarBase
{
public:
    NoderVarEnum()
    {
        _type = NoderVar::Type::Enum;
    }

private:
    QString _name;
    QString _value;
};

class NoderVarInterface : public NoderVarBase
{
public:
    NoderVarInterface()
    {
        _type = NoderVar::Type::Interface;
    }

private:
};