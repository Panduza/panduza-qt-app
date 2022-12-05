#pragma once

#include <QString>
#include <QGraphicsProxyWidget>
#include <PzaWidget.hpp>
#include <QGridLayout>

#include <PzaDoubleSpinBox.hpp>
#include <PzaSpinBox.hpp>
#include <PzaCheckBox.hpp>
#include <PzaLabel.hpp>
#include <PzaComboBox.hpp>
#include <PzaLineEdit.hpp>
#include <PzaLimits.hpp>
#include <Interface/Interface.hpp>
#include <Noder.hpp>

class GNode;
class Link;

class Pin : public PzaWidget
{
    Q_OBJECT

    public:
        const QString &name() const {return _name;}
        const PinProperty::Type &type() {return _pinType;}
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
        void kill(void);

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

        virtual bool isCompatible(Pin *to);

    protected:
        Pin();

        PinProperty::Type _pinType;
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

class PinVariable : public Pin
{
    Q_OBJECT
    
    public:
        PinVariable();

        const NoderVarProps &varProps(void) const {return _varProps;}
        void setSubType(const QString &subType) {_varProps.subType = subType; subTypeChanged();}
        virtual const QColor &plugColor(void) const {return Noder::Get().VarTypeColor(_varProps.type);}

        bool isCompatible(Pin *to) override;

    protected:
        NoderVarProps _varProps;

    signals:
        void subTypeChanged(void);
};