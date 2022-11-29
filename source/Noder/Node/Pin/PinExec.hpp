#pragma once

#include <Pin.hpp>

namespace PinDecl
{
    class Exec : public Pin
    {
        public:
            Exec();

            bool isCompatible(Pin *to) override;

        private:

    };
};