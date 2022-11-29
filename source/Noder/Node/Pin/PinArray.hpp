#pragma once

#include <Pin.hpp>

class PinArray : public PinVariable
{
    public:
        PinArray();

        bool isCompatible(Pin *to) override;
    
    private:
};

namespace PinDecl
{
    namespace Array
    {
        class Bool : public PinArray
        {
            public:
                Bool();

            private:
                std::vector<bool> _list;
        };

        class Int : public PinArray
        {
            public:
                Int();

            private:
                std::vector<int> _list;
        };

        class Float : public PinArray
        {
            public:
                Float();

            private:
                std::vector<double> _list;
        };

        class String : public PinArray
        {
            public:
                String();

            private:
                std::vector<QString> _list;
        };

        class Enum : public PinArray
        {
            public:
                Enum();

            private:
                std::vector<bool> _list;
        };

        class Interface : public PinArray
        {
            public:
                Interface();

            private:
                std::vector<bool> _list;
        };

        class Wildcard : public PinArray
        {
            public:
                Wildcard();

            private:
                std::vector<bool> _list;
        };
    }
}