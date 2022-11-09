#pragma once

#include <QGraphicsObject>
#include <QGraphicsProxyWidget>
#include <QFontMetrics>
#include <QPainter>
#include <QGridLayout>
#include <QPoint>
#include <QSvgRenderer>
#include <QFile>
#include <QFont>
#include <QCoreApplication>

#include <Node/Pin.hpp>
#include <PzaComboBox.hpp>
#include <PzaLabel.hpp>
#include <PzaUtils.hpp>
#include <PzaLineEdit.hpp>
#include <PzaMoreLess.hpp>
#include <PzaCheckBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaPropertyTable.hpp>
#include <PzaDoubleSpinBox.hpp>
#include <PzaColorBox.hpp>

class NoderScene;

// Node GUI model

class GNode : public QGraphicsObject
{
    Q_OBJECT

    public:
        GNode(const QString &name);

        const QString &name() const {return _name;}
        void setUserName(const QString &name);

        void process(void);
        virtual void exec(void) {};
        virtual GNode *branch(void) {return nullptr;}
        
        virtual void refreshNode(void) = 0;

        NoderScene *scene() const {return _scene;}
        void setScene(NoderScene *scene);
        PzaPropertyTable *propTable(void) const {return _propTable;}
        bool isInPlugzone(const QPoint &pos);
        void forEachInputPin(const std::function<void(Pin *pin)> &func);
        void forEachOutputPin(const std::function<void(Pin *pin)> &func);
        void forEachPin(const std::function<void(Pin *pin)> &func);
        void updateLinks(void);
        void setPos(const QPointF &pos);
        void setEternal(bool state) {_eternal = state;}
        bool isEternal(void) const {return _eternal;}

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;

        virtual void onEventConnect(void) {};
        virtual void onEventDisconnect(void) {};

        template <typename N>
        static GNode *CreateNode(void) {return new N();}
        PinValue *addPinFromType(NoderVar::Type type, const QString &name, PinProperty::Direction direction, int index = -1);
        int pinIndex(Pin *pin);
        void replacePin(Pin *oldPin, Pin *newPin);
        void deletePin(Pin *pin);
        void deleteValuePins(void);
        virtual void refreshUserName(const QString &name);

    protected:
        
        
        void setup(void);
        
        virtual void setType(NodeProperty::Type type);
        const NodeProperty::Type &nodeType(void) const {return _type;}

        template <typename N>
        N *addOutput(void)
        {
            return addPin<N>("", PinProperty::Direction::Output);
        }

        template <typename N>
        N *addOutput(const QString &name)
        {
            return addPin<N>(name, PinProperty::Direction::Output);
        }

        NodeProperty::Type _type;

        QRectF boundingRect() const override { return _nodebox; }
        
        void addProxyWidget(Pin *pin);
        void setOnTop(void);
        virtual void pinBoxSize(void) = 0;
        virtual void resizeBoxes(void) = 0;
        virtual void positionEntries(void) = 0;
        virtual void setWidgetSize(void) = 0;
        virtual void drawBoxes(QPainter *painter) = 0;
        inline bool needSpacing(void) {return (_inputPins.size() > 0 && _outputPins.size() > 0);}
        void setPinPlugzone(Pin *pin, const QPoint &origin);

        void drawValuePlug(QPainter *painter, PinValue *pin);
        void drawArrayPlug(QPainter *painter, PinDecl::Array *pin);
        void drawExecPlug(QPainter *painter, PinExec *pin);

        template <typename N>
        N *_addPin(const QString &name, PinProperty::Direction direction, int index = -1)
        {
            N *pin;

            pin = new N();

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
                    else {
                        _outputPins.push_back(pin);
                    }
                    break;
            }
            pin->setNode(this);
            pin->setDirection(direction);
            pin->setupProxyWidget();
            if (direction == PinProperty::Direction::Input)
                pin->setupWidgets();
            pin->setName(name);
            return pin;
        }

        template <typename N>
        N *addPin(const QString &name, PinProperty::Direction direction, int index = -1)
        {
            auto pin = _addPin<N>(name, direction, index);
            addProxyWidget(pin);
            return pin;
        }

        NoderScene *_scene;
        std::vector<Pin *> _inputPins;
        std::vector<Pin *> _outputPins;

        int _plugzone;
        int _plugRadius;
        QRect _nodebox;
        QRect _pinBox;
        int _entryMiny;
        int _pinBoxOffsetY;
        QString _name;
        QString _userName;
        int _boxRadius;
        QColor _boxColor;
        QPointF _prevPos;
        bool _eternal = false;

        PzaPropertyTable *_propTable = nullptr;
        PzaLabel *_propType = nullptr;
        PzaColorBox *_propBoxColor = nullptr;

    private:
        

    public slots:
        void setColor(const QColor &color);

    signals:
        void selected(void);
};