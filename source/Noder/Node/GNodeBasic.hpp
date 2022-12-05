#pragma once

#include "GNode.hpp"

class GNodeBasic : public GNode
{
    Q_OBJECT

    public:
        struct multiPin {
            int min;
            int max;
            NoderVarProps var;
            std::vector<PinRef *> *list;
            QGraphicsProxyWidget *proxy;
            PzaWidget *w;
            QString name;
            QString pinName;
        };

        GNodeBasic(const QString &name);

        void setTitleColor(const QColor &color);
        const QColor &defaultTitleColor(const NodeProperty::Type &type);
        const QString &userName() const {return _userName;}

        void refreshUserName(const QString &name) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void refreshNode(void) override;
        
        void forEachMultiPin(const std::function<void(const struct multiPin *s)> &func);

    protected:

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

        void setType(NodeProperty::Type type) override;

        template <typename N>
        N *addInput(void)
        {
            return addPin<N>("", PinProperty::Direction::Input);
        }

        template <typename N>
        N *addInput(const QString &name)
        {
            return addPin<N>(name, PinProperty::Direction::Input);
        }

        template <class N>
        void addMultiInput(std::vector<PinRef *> &list, const QString &name, const QString &pinName, const QString &subType = "", int min = 1, int max = -1)
        {
            PinRef *basePin;
            struct GNodeBasic::multiPin *s;

            if (min < 1)
                return;

            s = new multiPin;
            s->min = min;
            s->max = max;
            s->list = &list;
            s->name = name;
            s->pinName = pinName;

            basePin = addInput<N>(pinName + " " + QString::number(0));
            basePin->setSubType(subType);
            s->var = basePin->varProps();
            list.push_back(basePin);

            for (int i = 1; i < min; i++)
                addPintoMultiPin(s);
            createProxyMultiPin(s);
            _multiPinStructs.push_back(s);
        }

        template <class N>
        void addMultiInput(std::vector<PinRef *> &list, const QString &pinName, const QString &subType, int min = 1, int max = -1)
        {
            addMultiInput<N>(list, "", pinName, subType, min, max);
        }

        template <class N>
        void addMultiInput(std::vector<PinRef *> &list, const QString &pinName, int min = 1, int max = -1)
        {
            addMultiInput<N>(list, "", pinName, "", min, max);
        }

        struct multiPin *findMultiPinFromList(const std::vector<PinRef *> *list);

    private:
        struct title _title;
        int _spacingY;
        int _spacingMid;
        QRect _pinBoxIn;
        QRect _pinBoxOut;
        PzaLabel *_propName = nullptr;
        PzaColorBox *_propTitleColor = nullptr;
        PzaLineEdit *_propUserName = nullptr;
        std::vector<struct multiPin *> _multiPinStructs;

        void createProxyMultiPin(struct multiPin *s);
        void addPintoMultiPin(struct multiPin *s);
        void pinBoxSize(void) override;
        void resizeBoxes(void) override;
        void positionEntries(void) override;
        void setWidgetSize(void) override;
        void drawBoxes(QPainter *painter) override;
        void titleboxSize(void);
        void paint(QPainter *, QStyleOptionGraphicsItem const *opt = 0, QWidget *w = 0) override;

    public slots:
        void setUserName(void);
};