#pragma once

#include <QString>
#include <QGraphicsProxyWidget>
#include <PzaWidget.hpp>
#include <QGridLayout>

#include <PzaDoubleSpinBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaCheckBox.hpp>
#include <PzaLabel.hpp>
#include <PzaLineEdit.hpp>
#include <PzaLimits.hpp>
#include <Interface/Interface.hpp>
#include <Noder.hpp>
#include "NoderVar.hpp"

class GNode;
class Link;
class PinValue;

#define COMPATIBLE_TYPES(...)                                                  \
    const inline std::vector<NoderVar::Type> &compatibles() override           \
    {                                                                          \
        static std::vector<NoderVar::Type> map = {_valueType, __VA_ARGS__}; \
        return map;                                                            \
    }

class Pin : public PzaWidget
{
    Q_OBJECT

    public:
        const QString &name() const {return _name;}
        const PinProperty::Type &type() {return _type;}
        const PinProperty::Direction &direction() const {return _direction;}
        virtual void onEventConnect(void) {}
        virtual void onEventDisconnect(void) {}
        GNode *node(void) const {return _node;}

        QGraphicsProxyWidget *proxy() const {return _proxy;}
        const QPoint &pos() const {return _pos;}
        const QPointF &scenePos() const {return _scenePos;}
        const QSize &size() const {return _size;}

        void mousePressEvent(QMouseEvent *event) override;

        bool dead(void) {return _dead;}

        void setPos(const QPoint &pos) { _pos = pos; }
        void setScenePos(const QPointF &pos) { _scenePos = pos; }
        void setSize(const QSize &size) { _size = size; }
        void setPlugzone(const QRectF &plug)
        {
            QPointF center;

            _plugzone = plug;
            _plugcenter = _plugzone.center();
            center = _plugzone.center();
            _plugzoneIcon.setX(center.x() - 5);
            _plugzoneIcon.setY(center.y() - 5);
            _plugzoneIcon.setWidth(10);
            _plugzoneIcon.setHeight(10);
        }
        const QRectF &plugzone(void) const { return _plugzone; }
        const QRectF &plugzoneIcon(void) const { return _plugzoneIcon; }
        bool isInPlugArea(const QPointF &pos) const { return _plugzone.contains(pos); }
        const QPointF &plugCenter(void) const { return _plugcenter; }
        bool linked(void) const { return (_links.size() > 0); }
        static void CreateLink(Pin *from);
        static void CreateLink(Pin *from, Pin *to);
        static void CreateLink(Pin *from, const QPointF &pos);
        void connectLink(Link *link);
        void moveLinks(void);
        void disconnectLink(const QPointF &pos);
        std::vector<GNode *> &linkedNodes(void) { return _linkedNodes; }
        std::vector<Pin *> &linkedPins(void) { return _linkedPins; }
        void forEachLinkedPin(const std::function<void(Pin *pin)> &func);
        void forEachLinkedNode(const std::function<void(GNode *node)> &func);
        void setNode(GNode *node) { _node = node; }
        void removeLink(Link *link);
        void removeLinks(void);
        bool isInput(void) { return _direction == PinProperty::Direction::Input; }
        bool isOutput(void) { return _direction == PinProperty::Direction::Output; }
        static PinProperty::Direction OppositeDirection(PinProperty::Direction dir)
        {
            if (dir == PinProperty::Direction::Input)
                return PinProperty::Direction::Output;
            return PinProperty::Direction::Input;
        }

        virtual void setupProxyWidget(void);
        virtual void setupWidgets(void) {}

        void hideWidgets(void);
        void showWidgets(void);
        void setName(const QString &name);
        void setDirection(PinProperty::Direction direction) { _direction = direction; }

        virtual bool isCompatible(Pin *to) = 0;

        ~Pin();

    protected:
        Pin();

        PinProperty::Type _type;
        QString _name;
        PinProperty::Direction _direction;
        GNode *_node = nullptr;
        QPoint _pos;
        QPointF _scenePos;
        QGraphicsProxyWidget *_proxy = nullptr;
        QGridLayout *_grid = nullptr;
        QSize _size;
        QRectF _plugzone;
        QRectF _plugzoneIcon;
        QPointF _plugcenter;
        std::vector<Link *> _links;
        std::vector<GNode *> _linkedNodes;
        std::vector<Pin *> _linkedPins;
        PzaLabel *_label;

        void forEachLink(const std::function<void(Link *link)> &func);

    private:
        bool _dead = false;
};

class PinValue : public Pin
{
    public:
        PinValue();

        const NoderVar::Type &valueType(void) const {return _valueType;}
        virtual const QColor &plugColor(void) const {return Noder::Get().varTypeColor(_valueType);}
        static PinValue *PinTypeToObj(const NoderVar::Type type);

        bool isCompatible(Pin *to) override;

        const virtual inline std::vector<NoderVar::Type> &compatibles(void) {
            static std::vector<NoderVar::Type> map = {}; \
            return map; 
        };

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
        virtual void setValue(const std::vector<PinValue *> &val)
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
                    static_cast<PinValue *>(pin)->setValue(val);
                });
            }
        }

    protected:
        NoderVar::Type _valueType;
};

class PinExec : public Pin
{
    public:
        PinExec();

        bool isCompatible(Pin *to) override;

    private:

};

namespace PinDecl
{
    class Wildcard : public PinValue
    {
        private:
            COMPATIBLE_TYPES
            (
                NoderVar::Type::Bool,
                NoderVar::Type::Int,
                NoderVar::Type::Float,
                NoderVar::Type::String,
                NoderVar::Type::Enum,
                NoderVar::Type::Interface
            )
    };

    class Float : public PinValue
    {
        public:
            Float();

            void setupWidgets(void) override;

            const NoderVarBase &data(void) const { return _data; }

            void setValue(const double value) override { _data.setValue(value); }
            void setValue(const int val) override;
            void setValue(const bool val) override;
            void setValue(const QString &val) override;

            double value(void)
            {
                if (linked())
                    return _data.value();
                return _box->value();
            }

        private:
            COMPATIBLE_TYPES
            (
                NoderVar::Type::Bool,
                NoderVar::Type::Int,
                NoderVar::Type::String
            )
            NoderVarFloat _data;
            PzaDoubleSpinBox *_box;
    };

    class Int : public PinValue
    {
    public:
        Int();

        void setupWidgets(void) override;
        
        const NoderVarBase &data(void) const { return _data; }

        void setValue(const int value) override {_data.setValue(value);}
        void setValue(const double val) override;
        void setValue(const bool val) override;
        void setValue(const QString &val) override;

        int value(void)
        {
            if (linked())
                return _data.value();
            return _box->value();
        }

    private:
        COMPATIBLE_TYPES
        (
            NoderVar::Type::Float,
            NoderVar::Type::Bool,
            NoderVar::Type::String
        )
        NoderVarInt _data;
        PzaSpinBox *_box;
    };

    class Bool : public PinValue
    {
        public:
            Bool();

            void setupWidgets(void) override;

            const NoderVarBase &data(void) const {return _data;}

            void setValue(const bool value) override {_data.setValue(value);}

            void setValue(const double val) override;
            void setValue(const int val) override;
            void setValue(const QString &val) override;

            bool value(void)
            {
                if (linked())
                    return _data.value();
                return _box->isChecked();
            }

        private:
            COMPATIBLE_TYPES
            (
                NoderVar::Type::Float,
                NoderVar::Type::Int,
                NoderVar::Type::String
            )
            NoderVarBool _data;
            PzaCheckBox *_box;
    };

    class String : public PinValue
    {
        public:
            String();

            void setupWidgets(void) override;

            const NoderVarBase &data(void) const {return _data;}

            void setValue(const QString &value) {_data.setValue(value);}
            void setValue(const double value) override;
            void setValue(const int value) override;
            void setValue(const bool value) override;

            const QString value(void)
            {
                if (linked())
                    return _data.value();
                return _box->text();
            }

        private:
            COMPATIBLE_TYPES
            (
                NoderVar::Type::Float,
                NoderVar::Type::Int,
                NoderVar::Type::Bool
            )
            NoderVarString _data;
            PzaLineEdit *_box;
    };

    class Enum : public PinValue
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
            COMPATIBLE_TYPES()

            QString _enumName;
            std::vector<QString> _list;

        public slots:
            void modifyEnumName(const QString &name);

        signals:
            void initialized(void);
    };

    class Array : public PinValue
    {
        public:
            Array();

            const auto &list(void) {return _list;}
            const NoderVar::Type &elemType(void) {return _elemType;}
            bool isCompatible(Pin *pin) override;
            void setValue(const std::vector<PinValue *> &vec) override
            {
                _list = vec;
            }
            void setElemType(const NoderVar::Type &type)
            {
                _elemType = type;
            }
            void setOriginalType(const NoderVar::Type &type)
            {
                _oriElemType = type;
                _elemType = type;
            }
            void onEventConnect(void) override;
            void onEventDisconnect(void) override;

            const QColor &plugColor(void) const override {return Noder::Get().varTypeColor(_elemType);}

        private:
            COMPATIBLE_TYPES()

            NoderVar::Type _oriElemType = NoderVar::Type::Wildcard;
            NoderVar::Type _elemType = NoderVar::Type::Wildcard;
            std::vector<PinValue *> _list;
    };

    class Interface : public PinValue
    {
        public:
            Interface();

            PzaInterface::Interface *object(void) { return _object; }

            bool isCompatible(Pin *pin) override;

            void setValue(PzaInterface::Interface *val) override { _object = val; }

        private:
            COMPATIBLE_TYPES()

            PzaInterface::Interface *_object = nullptr;
    };

}
