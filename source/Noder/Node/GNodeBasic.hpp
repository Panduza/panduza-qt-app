#pragma once

#include "GNode.hpp"

class GNodeBasic : public GNode
{
    Q_OBJECT

    public:
        struct multiPin {
            int min;
            int max;
            PinProperty::Type type;
            std::vector<Pin *> *list;
            QGraphicsProxyWidget *proxy;
            PzaWidget *w;
            QString name;
            QString pinName;
        };

        GNodeBasic(const QString &name);
        ~GNodeBasic();

        void setTitleColor(const QColor &color);
        const QColor &defaultTitleColor(const NodeProperty::Type &type);
        const QString &userName() const {return _userName;}

        void refreshUserName(const QString &name) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void refreshNode(void) override;
        
        void forEachMultiPin(const std::function<void(struct multiPin *s)> &func);

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

        template <typename N>
        void addMultiInput(const QString &name, const QString &pinName, std::vector<Pin *> *list, int min, int max)
        {
            Pin *basePin;
            struct GNodeBasic::multiPin *s;

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

            for (int i = 1; i < min; i++)
                addPintoMultiPin(s);
            createProxyMultiPin(s);
            _multiPinStructs.push_back(s);
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

        struct multiPin *findMultiPinFromList(std::vector<Pin *> *list);

    private:
        struct title *_title = nullptr;
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