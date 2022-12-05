#pragma once

#include <Pin.hpp>

class PinRef : public PinVariable
{
    public:
        PinRef();

        bool isCompatible(Pin *to) override;

        virtual void setValue(const bool val)
        {
            (void)val;
            qDebug() << "Wooh bad idea";
        };
        virtual void setValue(const double val)
        {
            (void)val;
            qDebug() << "Wooh bad idea";
        };
        virtual void setValue(const int val)
        {
            (void)val;
            qDebug() << "Wooh bad idea";
        };
        virtual void setValue(const QString &val)
        {
            (void)val;
            qDebug() << "Wooh bad idea";
        };

        virtual void setValue(PzaInterface::Interface *val)
        {
            (void)val;
            qDebug() << "Wooh bad idea";
        };


        template <typename T>
        void sendValue(T val)
        {
            if (linked())
            {
                forEachLinkedPin([val](Pin *pin) {
                    static_cast<PinRef *>(pin)->setValue(val);
                });
            }
        }
};

namespace PinDecl
{
    namespace Ref
    {
        class Wildcard : public PinRef
        {

        };

        class Bool : public PinRef
        {
            public:
                Bool();

                void setupWidgets(void) override;

                void setValue(const bool value) {_value = value;}

                void setValue(const double val);
                void setValue(const int val);
                void setValue(const QString &val);

                bool value(void)
                {
                    if (linked())
                        return _value;
                    return _box->isChecked();
                }

            private:
                bool _value;
                PzaCheckBox *_box;
        };

        class Float : public PinRef
        {
            public:
                Float();

                void setupWidgets(void) override;

                void setValue(const double value) {_value = value;}
                void setValue(const int val);
                void setValue(const bool val);
                void setValue(const QString &val);

                double value(void)
                {
                    if (linked())
                        return _value;
                    return _box->value();
                }

            private:
                double _value;
                PzaDoubleSpinBox *_box;
        };

        class Int : public PinRef
        {
        public:
            Int();

            void setupWidgets(void) override;

            void setValue(const int value) {_value = value;}
            void setValue(const double val);
            void setValue(const bool val);
            void setValue(const QString &val);

            int value(void)
            {
                if (linked())
                    return _value;
                return _box->value();
            }

        private:
            int _value;
            PzaSpinBox *_box;
        };

        class String : public PinRef
        {
            public:
                String();

                void setupWidgets(void) override;

                void setValue(const QString &value) {_value = value;}
                void setValue(const double value);
                void setValue(const int value);
                void setValue(const bool value);

                const QString value(void)
                {
                    if (linked())
                        return _value;
                    return _box->text();
                }

            private:
                QString _value;
                PzaLineEdit *_box;
        };

        class Enum : public PinRef
        {
            Q_OBJECT

            public:
                Enum();

                void setupWidgets(void) override;

                const QString &enumName(void) {return _enumName;}
                void initialize(const QString &name);
                const std::vector<QString> &list(void) {return _list;}

                bool isCompatible(Pin *pin) override;

            private:
                QString _enumName;
                PzaComboBox *_box;
                std::vector<QString> _list;

            public slots:
                void modifyEnumName(const QString &name);

            signals:
                void initialized(void);
        };

        class Interface : public PinRef
        {
            public:
                Interface();

                PzaInterface::Interface *object(void) { return _object; }

                bool isCompatible(Pin *pin) override;

                void setValue(PzaInterface::Interface *val) { _object = val; }

            private:
                PzaInterface::Interface *_object = nullptr;
        };
    }
}