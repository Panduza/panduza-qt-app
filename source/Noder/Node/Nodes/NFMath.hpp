#pragma once

#include <GNodeOp.hpp>

class ABasicOperation : public GNodeOp
{
    protected:
        ABasicOperation(const QString &name);

        virtual double mathOp(double a, double b) = 0;

        void exec(void) override;

        PinDecl::Ref::Float *_result;
        std::vector<PinRef *> _list;
};

class Addition : public ABasicOperation
{
    public:
        Addition() : ABasicOperation("Add") {};

        double mathOp(double a, double b) override {return a + b;}
};

class Substraction : public ABasicOperation
{
    public:
        Substraction() : ABasicOperation("Substract") {};

        double mathOp(double a, double b) override {return a - b;}
};

class Multiply : public ABasicOperation
{
    public:
        Multiply() : ABasicOperation("Multiply") {};
      
        double mathOp(double a, double b) override {return a * b;}
};

class Divide : public ABasicOperation
{
    public:
        Divide() : ABasicOperation("Divide") {};

        double mathOp(double a, double b) override {return a / b;}
};

class Modulo : public ABasicOperation
{
    public:
        Modulo() : ABasicOperation("Modulo") {};

        double mathOp(double a, double b) override {return a + b;}
};