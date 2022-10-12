#pragma once

#include <QGraphicsObject>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QFontMetrics>
#include <QPainter>
#include <QGridLayout>
#include <QWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPoint>
#include <QLineEdit>
#include <QComboBox>
#include <QSvgRenderer>
#include <QFile>
#include <QPushButton>

#include <Node/Pin.hpp>
#include <PzaComboBox.hpp>
#include <PzaUtils.hpp>

class NoderScene;

// Node GUI model

class GNode : public QGraphicsObject
{
    Q_OBJECT

    public:
        struct multiPin {
            int min;
            int max;
            PinProperty::Type type;
            std::vector<Pin *> *list;
            QGraphicsProxyWidget *proxy;
            QWidget *w;
            QString name;
            QString pinName;
        };

        GNode(const QString &name);

        inline const QString &name() const {return _name;}

        void process(void);
        virtual void exec(void) {};
        virtual GNode *branch(void) = 0;

        inline NoderScene *scene() const {return _scene;}
        void setScene(NoderScene *scene);
        void refreshNode(void);
        bool isInPlugzone(const QPoint &pos);
        void forEachInputPin(std::function<void(Pin *pin)> func);
        void forEachOutputPin(std::function<void(Pin *pin)> func);
        void forEachPin(std::function<void(Pin *pin)> func);
        void forEachMultiPin(std::function<void(struct multiPin *s)> func);
        void updateLinks(void);

        virtual void onEventConnect(void) {};
        virtual void onEventDisconnect(void) {};

        template <typename N>
        static GNode *createNode(void) {return new N();}

    protected:
        enum class PlugType {
            Value = 0,
            Array,
            Exec
        };

        struct title {
            QRect box;
            QSize size;
            QFont font;
            QRect fontbox;
            QPoint fontpos;
            QColor fontcolor;
            QSize offset;
            QColor boxcolor;
        };

        struct plugIconData {
            QString filename;
            QString data;
        };

        struct plugIcon {
            struct plugIconData plugC;
            struct plugIconData plugNc;
        };
        
        ~GNode();

        void setup(void);
        
        struct multiPin *findMultiPinFromList(std::vector<Pin *> *list);
        void deletePin(Pin *pin);
        void createLink(Pin *from, Pin *to);
        inline const NodeProperty::Type &nodeType(void) const {return _type;}
        const QColor &titleColor(const NodeProperty::Type &type);
        const QColor &plugColor(PinProperty::Type type);

        template <typename N>
        N *addInput(const QString &name)
        {
            return addPin<N>(name, PinProperty::Direction::Input);
        }

        template <typename N>
        N *addOutput(const QString &name)
        {
            return addPin<N>(name, PinProperty::Direction::Output);
        }

        template <typename N>
        void addMultiInput(const QString &name, const QString &pinName, std::vector<Pin *> *list, int min, int max)
        {
            Pin *basePin;
            struct GNode::multiPin *s;

            if (min < 1)
                return;

            s = new struct multiPin;
            s->min = min;
            s->max = max;
            s->list = list;
            s->name = name;
            s->pinName = pinName;

            basePin = addPin<N>(pinName + " " + QString::number(0), PinProperty::Direction::Input);
            s->type = basePin->type();
            list->push_back(basePin);
            _multiPinStructs.push_back(s);

            for (int i = 1; i < min; i++)
                addPintoMultiPin(s);

            createProxyMultiPin(s);
        }
        template <typename N>
        void addMultiInput(const QString &name, const QString &pinName, std::vector<Pin *> *list, unsigned int min)
        {
            addMultiInput<N>(name, pinName, list, min, -1);
        }
        template <typename N>
        void addMultiInput(const QString &pinName, std::vector<Pin *> *list)
        {
            addMultiInput<N>("", pinName, list, 2);
        }
        template <typename N>
        void addMultiInput(const QString &pinName, std::vector<Pin *> *list, unsigned int min)
        {
            addMultiInput<N>("", pinName, list, min, -1);
        }
        template <typename N>
        void addMultiInput(std::vector<Pin *> *list, unsigned int min)
        {
            addMultiInput<N>("", list, min, -1);
        }
        template <typename N>
        void addMultiInput(std::vector<Pin *> *list, unsigned int min, unsigned int max)
        {
            addMultiInput<N>("", list, min, max);
        }
        template <typename N>
        void addMultiInput(std::vector<Pin *> *list)
        {
            addMultiInput<N>("", "", list, 2, -1);
        }

        Pin *addPinFromType(PinProperty::Type type, const QString &name, PinProperty::Direction direction, int index = -1);

        bool _hasTitle = true;
        bool _hasWidgets = true;
        NodeProperty::Type _type;

    private:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
        QRectF boundingRect() const override { return _nodebox; }
        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
        void paint(QPainter *, QStyleOptionGraphicsItem const *, QWidget *) override;

        void createProxyWidget(Pin *pin);
        void setOnTop(void);
        void pinBoxSize(void);
        void titleboxSize(void);
        void resizeBoxes(void);
        void positionEntries(void);
        void setWidgetSize(void);
        void drawBoxes(QPainter *painter);
        inline bool needSpacing(void) {return (_inputPins.size() > 0 && _outputPins.size() > 0);}
        void setPinPlugzone(Pin *pin, const QPoint &origin);
        void createProxyMultiPin(struct multiPin *s);
        void addPintoMultiPin(struct multiPin *s);
        QString loadPlugIcon(const QString &filename);
        struct plugIcon initPlugType(PlugType type);
        QByteArray loadColorValue(const PinProperty::Type &type, bool linked);
        QByteArray loadColorCValue(const PinProperty::Type &type);
        QByteArray loadColorNcValue(const PinProperty::Type &type);
        void drawValuePlug(QPainter *painter, Pin *pin);
        void drawArrayPlug(QPainter *painter, Pin *pin);
        void drawExecPlug(QPainter *painter, Pin *pin);

        template <typename N>
        N *_addPin(const QString &name, PinProperty::Direction direction, int index = -1)
        {
            N *pin;

            pin = new N();
            pin->setName(name);
            pin->setDirection(direction);
            pin->setPlugColor();
            pin->setNode(this);

            switch (direction) {
                case PinProperty::Direction::Input:
                    if (index != -1)
                        _inputPins.insert(_inputPins.begin() + index, pin);
                    else
                        _inputPins.push_back(pin);
                    break;
                case PinProperty::Direction::Output:
                    if (index != -1)
                        _outputPins.insert(_outputPins.begin() + index, pin);
                    else
                        _outputPins.push_back(pin);
                    break;
            }
            return pin;
        }

        template <typename N>
        N *addPin(const QString &name, PinProperty::Direction direction, int index = -1)
        {
            auto pin = _addPin<N>(name, direction, index);
            createProxyWidget(pin);
            return pin;
        }

        static inline std::unordered_map<PlugType, struct plugIcon> _mapPlugFiles;
        static inline std::unordered_map<PinProperty::Type, QByteArray> _mapPlugColoredNcIcon;
        static inline std::unordered_map<PinProperty::Type, QByteArray> _mapPlugColoredCIcon;

        NoderScene *_scene;
        std::vector<Pin *> _inputPins;
        std::vector<Pin *> _outputPins;
        std::vector<struct multiPin *> _multiPinStructs;

        struct title *_title = nullptr;

        int _spacing_y;
        int _spacing_mid;
        int _plugzone;
        int _plug_radius;
        int _entry_miny;
        int _pinBox_offsety;
        QRect _nodebox;
        QRect _pinBox;
        QRect _pinBox_in;
        QRect _pinBox_out;
        QString _name;
};