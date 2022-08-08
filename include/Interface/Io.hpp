#ifndef IO_HPP
#define IO_HPP

#include <Interface.hpp>

class Io : public Interface
{
    public:
        Io(Group *group, const QString &name);
        Io() = default;
        ~Io() = default;

        const QString &direction(void) const {return _direction;}
        const int &value(void) const {return _value;}

        const e_InterfaceType type(void) override {return E_TYPE_IO;}

        void sendDirection(const QString &direction);
        void sendValue(const int value);

    private:
        QString _direction;
        int _value;

        void setDirection(const QString &direction) {_direction = direction;}
        void setValue(const int &value) {_value = value;}

        void parseDirection(const QJsonDocument &json) {setDirection(PzaJSON::keyToString(json, "direction"));}
        void parseValue(const QJsonDocument &json) {setValue(PzaJSON::keyToInt(json, "value"));}
};

#endif