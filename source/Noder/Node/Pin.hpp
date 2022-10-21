#pragma once

#include <QString>
#include <QGraphicsProxyWidget>
#include <PzaWidget.hpp>
#include <QGridLayout>

#include <PzaLimits.hpp>
#include <NoderDataBase.hpp>

class GNode;
class Link;

#define COMPATIBLE_TYPES(...) \
const inline std::vector<PinProperty::Type> &compatibles() override \
{ \
    static std::vector<PinProperty::Type> map = {_type, __VA_ARGS__}; \
    return map; \
}

class Pin : public PzaWidget
{
    Q_OBJECT

    public:
        const QString &name() const {return _name;}
        const PinProperty::Type &type() {return _type;}
        const PinProperty::Direction &direction() const {return _direction;}
        const virtual inline std::vector<PinProperty::Type> &compatibles(void) = 0;
        virtual void onEventConnect(void) {};
        virtual void onEventDisconnect(void) {};
        GNode *node(void) const {return _node;}

        QGraphicsProxyWidget *proxy() const {return _proxy;}
        QGridLayout *grid() const {return _grid;}
        const QPoint &pos() const {return _pos;}
        const QPointF &scenePos() const {return _scenePos;}
        const QSize &size() const {return _size;}

        void mousePressEvent(QMouseEvent *event) override;

        bool dead(void) {return _dead;}

        void setPos(const QPoint &pos) {_pos = pos;}
        void setScenePos(const QPointF &pos) {_scenePos = pos;}
        void setProxy(QGraphicsProxyWidget *proxy) {_proxy = proxy;}
        void setGrid(QGridLayout *grid) {_grid = grid;}
        void setSize(const QSize &size) {_size = size;}
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
        std::vector<GNode *> &linkedNodes(void) {return _linkedNodes;}
        std::vector<Pin *> &linkedPins(void) {return _linkedPins;}
        void forEachLinkedPin(std::function<void(Pin *pin)> func);
        void forEachLinkedNode(std::function<void(GNode *node)> func);
        const QColor &plugColor(void) {return _plugColor;}
        virtual void setPlugColor(void) {_plugColor = NBD_INST.plugColor(_type);}
        void setNode(GNode *node) {_node = node;}
        void removeLink(Link *link);
        void removeLinks(void);
        bool isInput(void) {return _direction == PinProperty::Direction::Input;}
        bool isOutput(void) {return _direction == PinProperty::Direction::Output;}

        static PinProperty::Direction OppositeDirection(PinProperty::Direction direction)
        {
            if (direction == PinProperty::Direction::Input)
                return PinProperty::Direction::Output;
            else
                return PinProperty::Direction::Input;
        }

        void hideWidgets(void);
        void showWidgets(void);
        void setName(const QString &name);
        void setDirection(PinProperty::Direction direction) {_direction = direction;}

        virtual bool isCompatible(Pin *to);

        virtual void setValue(const bool val) {(void)val;qDebug() << "Wooh bad idea";};
        virtual void setValue(const double val) {(void)val;qDebug() << "Wooh bad idea";};
        virtual void setValue(const int val) {(void)val;qDebug() << "Wooh bad idea";};
        virtual void setValue(const QString &val) {(void)val;qDebug() << "Wooh bad idea";};
        virtual void setValue(const std::vector<Pin *> &val) {(void)val;qDebug() << "Wooh bad idea";};
        virtual void setValue(PzaInterface::Interface *val) {(void)val;qDebug() << "Wooh bad idea";};
        //virtual void setValue(Interface *val) {qDebug() << "Wooh bad idea";};

        template<typename T>
        void sendValue(T val) {
            if (linked()) {
                forEachLinkedPin([val](Pin *pin){
                    pin->setValue(val);
                });
            }
        }
        
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
        QColor _plugColor;

        void forEachLink(std::function<void(Link *link)> func);

    private:
        bool _dead = false;

    signals:
        void askWidget(void);
        void nameChanged(const QString &name);
};

namespace PinDecl {

class Wildcard : public Pin
{
    public:
        Wildcard();
        ~Wildcard() = default;

    private:
        COMPATIBLE_TYPES
        (
            PinProperty::Type::Bool,
            PinProperty::Type::Int,
            PinProperty::Type::Float,
            PinProperty::Type::String,
            PinProperty::Type::Enum,
            PinProperty::Type::Interface
        )
};

class Float : public Pin
{
    public:
        Float(double value = 0.f, double min = _Float_MIN, double max = _Float_MAX, unsigned int decimals = 2);
        ~Float() = default;

        void setValue(const double value) override {_value = value;}
        void setValue(const int val) override;
        void setValue(const bool val) override;
        void setValue(const QString &val) override;

        void setMin(const double value) {_min = value;}
        void setMax(const double value) {_max = value;}
        void setDecimals(const unsigned int value) {_decimals = value;}

        double value(void) const {return _value;}
        double min(void) const {return _min;}
        double max(void) const {return _max;}
        unsigned int decimals(void) const {return _decimals;}

        double value(void)
        {
            if (!linked())
                askWidget();
            return _value;
        }

    private:
        COMPATIBLE_TYPES
        (
            PinProperty::Type::Bool,
            PinProperty::Type::Int,
            PinProperty::Type::String
        )
        double _value;
        double _min;
        double _max;
        unsigned int _decimals;
};

class Int : public Pin
{
    public:
        Int(int value = 0, int min = _Int_MIN, int max = _Int_MAX);
        ~Int() = default;

        void setValue(const int value) override {_value = value;}
        void setValue(const double val) override;
        void setValue(const bool val) override;
        void setValue(const QString &val) override;

        void setMin(const int value) {_min = value;}
        void setMax(const int value) {_max = value;}

        int value(void) const {return _value;}
        int min(void) const {return _min;}
        int max(void) const {return _max;}

        int value(void)
        {
            if (!linked())
                askWidget();
            return _value;
        }

    private:
        COMPATIBLE_TYPES
        (
            PinProperty::Type::Float,
            PinProperty::Type::Bool,
            PinProperty::Type::String
        )
        int _value;
        int _min;
        int _max;
};

class Bool : public Pin
{
    public:
        Bool(bool value = false);
        ~Bool() = default;

        void setValue(const bool value) override {_value = value;}

        void setValue(const double val) override;
        void setValue(const int val) override;
        void setValue(const QString &val) override;

        bool value(void)
        {
            if (!linked())
                askWidget();
            return _value;
        }

    private:
        COMPATIBLE_TYPES
        (
            PinProperty::Type::Float,
            PinProperty::Type::Int,
            PinProperty::Type::String
        )
        bool _value;
};

class String : public Pin
{
    public:
        String(const QString &value = "");
        ~String() = default;

        void setValue(const QString &value) override;
        void setValue(const double value) override;
        void setValue(const int value) override;
        void setValue(const bool value) override;

        const QString &value(void)
        {
            if (!linked())
                askWidget();
            return _value;
        }

    private:
        COMPATIBLE_TYPES
        (
            PinProperty::Type::Float,
            PinProperty::Type::Int,
            PinProperty::Type::Bool
        )
        QString _value;
};

class Enum : public Pin
{
    Q_OBJECT
    
    public:
        Enum();
        ~Enum() = default;

        const QString &enumName(void) {return _enumName;}
        void initialize(const QString &name);
        const std::vector<QString> &list(void) {return _list;}

        bool isCompatible(Pin *pin) override;

    private:
        COMPATIBLE_TYPES
        (
        )

        QString _enumName;
        std::vector<QString> _list;

    public slots:
        void modifyEnumName(const QString &name);

    signals:
        void initialized(void);
};

class Array : public Pin
{
    public:
        Array();
        ~Array() = default;

        const auto &list(void) {return _list;}
        const PinProperty::Type &elemType(void) {return _elemType;}

        bool isCompatible(Pin *pin) override;

        void setValue(const std::vector<Pin *> &vec) override
        {
            _list = vec;
        }

        void setElemType(const PinProperty::Type &type)
        {
            _elemType = type;
            setPlugColor();
        }

        void setOriginalType(const PinProperty::Type &type)
        {
            _oriElemType = type;
            _elemType = type;
        }

        void onEventConnect(void) override;
        void onEventDisconnect(void) override;
        virtual void setPlugColor(void) {_plugColor = NBD_INST.plugColor(_elemType);}

    private:
        COMPATIBLE_TYPES
        (
        )

        PinProperty::Type _oriElemType = PinProperty::Type::Wildcard;
        PinProperty::Type _elemType = PinProperty::Type::Wildcard;
        std::vector
        <
            Pin *
        > _list;
};

class Exec : public Pin
{
    public:
        Exec();
        ~Exec() = default;

    private:
        COMPATIBLE_TYPES
        (
        )
};

class Interface : public Pin
{
    public:
        Interface();
        ~Interface() = default;

        PzaInterface::Interface *object(void) {return _object;}

        bool isCompatible(Pin *pin) override;

        void setValue(PzaInterface::Interface *val) override {_object = val;}

    private:
        COMPATIBLE_TYPES
        (
        )
        
        PzaInterface::Interface *_object = nullptr;
};

}