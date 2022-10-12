#pragma once

#include <GNodeOp.hpp>

class ABasicOperation : public GNodeOp
{
    protected:
        ABasicOperation(const QString &name);
        ~ABasicOperation() = default;

        virtual double mathOp(double a, double b) = 0;

        void exec(void) override;

        PinDecl::Float *_result;
        std::vector<Pin *> _list;
};

class Addition : public ABasicOperation
{
    public:
        Addition() : ABasicOperation("Add") {};
        ~Addition() = default;

        double mathOp(double a, double b) override {return a + b;}
};

class Substraction : public ABasicOperation
{
    public:
        Substraction() : ABasicOperation("Substract") {};
        ~Substraction() = default;

        double mathOp(double a, double b) override {return a - b;}
};

class Multiply : public ABasicOperation
{
    public:
        Multiply() : ABasicOperation("Multiply") {};
        ~Multiply() = default;
      
        double mathOp(double a, double b) override {return a * b;}
};

class Divide : public ABasicOperation
{
    public:
        Divide() : ABasicOperation("Divide") {};
        ~Divide() = default;

        double mathOp(double a, double b) override {return a / b;}
};

class Modulo : public ABasicOperation
{
    public:
        Modulo() : ABasicOperation("Modulo") {};
        ~Modulo() = default;

        double mathOp(double a, double b) override {return a + b;}
};