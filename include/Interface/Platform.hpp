#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <Interface.hpp>

class Platform : public Interface
{
    public:
        Platform(Group *group, const QString &name);
        ~Platform() = default;

        const e_InterfaceType type(void) override {return E_TYPE_PLATFORM;}
};

#endif