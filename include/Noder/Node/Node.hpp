#ifndef NODE_HPP
#define NODE_HPP

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>

#include <NoderScene.hpp>
#include <NodeEntry.hpp>
#include <NodeProperty.hpp>

class Node : public QGraphicsObject
{
    enum { Type = UserType + 1 };

    public:
        QString name(void) const { return _name; }
        NoderScene *scene(void) const { return _scene; }
        PanduzaEngine *engine(void) const { return _scene->engine(); }
        void updateLinks(void);
        std::vector<NodeEntry *> entriesIn() {return _entries_in;}
        std::vector<NodeEntry *> entriesOut() {return _entries_out;}
        bool isInPlugzone(QPoint pos);
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
        void refreshNode(void);
        QRectF boundingRect() const override { return _nodebox; }

        int type() const override {return Type;}

        template<typename T>
        static Node *createChildNode(NoderScene *scene) {return new T(scene);}

        virtual bool exec(void) = 0;
        virtual Node *branch(void) = 0;

        void forEachProperties(std::function<void(NodeProperty *property)> f)
        {
            for (auto property: _properties) {
                f(property);
            }
        }
        
        template<class N>
        N *addNodeEntryInput(QString name) {return addNodeEntry<N>(name, NodeEntryDirection::Input);}
        template<class N>
        N *addNodeEntryInput(void) {return addNodeEntry<N>(NodeEntryDirection::Input);}
        template<class N>
        N *addNodeEntryOutput(QString name) {return addNodeEntry<N>(name, NodeEntryDirection::Output);}
        template<class N>
        N *addNodeEntryOutput(void) {return addNodeEntry<N>(NodeEntryDirection::Output);}

        template<class N>
        N *addNodeProperty(QString name)
        {
            N *obj = new N(name, this);
            _properties.push_back(obj);
            return obj;
        }
        template<class N>
        N *addNodeProperty() {return addNodeProperty<N>(nullptr);}

        void deleteEntry(NodeEntry *entry)
        {
            NodeEntry *found = nullptr;

            auto removeFromList = [&](NodeEntry *entry, std::vector<NodeEntry *>& list) {
                auto it = find(list.begin(), list.end(), entry);
                delete entry;
                list.erase(it);
            };

            if (entry->direction() == NodeEntryDirection::Input)
                removeFromList(entry, _entries_in);
            else
                removeFromList(entry, _entries_out);

        }

    protected:
        Node(NoderScene *scene, QString name);
        ~Node();

        void setTitleColor(QString color) {_titlecolor = QColor(color);}

        QString _name;
        QColor _titlecolor;
        
        std::vector<NodeEntry *> _entries_in;
        std::vector<NodeEntry *> _entries_out;
        std::vector<NodeProperty *> _properties;

    private:
        template<class N>
        N *addNodeEntry(QString name, enum NodeEntryDirection direction)
        {
            N *obj = new N(name, direction, this);
            switch (direction) {
                case Input:
                    _entries_in.push_back(obj);
                    break ;
                case Output:
                    _entries_out.push_back(obj);
                    break ;
            }
            return obj;
        }
        template<class N>
        N *addNodeEntry(enum NodeEntryDirection direction) {return addNodeEntry<N>(nullptr, direction);}

        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

        void paint(QPainter *, QStyleOptionGraphicsItem const *, QWidget *) override;

        void drawBoxes(QPainter *painter);
        void drawTitle(QPainter *painter);
        void drawPlugs(QPainter *painter);

        bool isExecEntry(NodeEntry *entry);

        NoderScene *_scene;

        QRect _nodebox;
        QRect _fontbox;

        QPoint _fontpos;

        int _entrybox_offsety;
        int _spacing_y;
        int _spacing_mid;
        int _entry_miny;
        int _plug_radius;
        int _plugzone;
        QRect _entrybox;
        QRect _entrybox_in;
        QRect _entrybox_out;
        QRect _propertybox;
        QRect _titlebox;
        QSize _title_offset;
        QFont _titlefont;

        void entryboxSize(void);
        void propertyboxSize(void);
        void titleboxSize(void);
        void resizeBoxes(void);
        void setWidgetSize(void);
        void positionEntries(void);
        bool needSpacing(void) {return (_entries_in.size() > 0 && _entries_out.size() > 0);}
        void setEntryPlugzoneLeft(NodeEntry *entry, QPoint origin);
        void setEntryPlugzoneRight(NodeEntry *entry, QPoint origin);
        void drawRoundPlug(QPainter *painter, NodeEntry *entry);
        void drawExecPlug(QPainter *painter, NodeEntry *entry);
        void setOnTop(void);
        void forEachEntries(std::function<void(NodeEntry *entry)> f)
        {
            for (auto entry: _entries_in) {
                f(entry);
            }
            for (auto entry: _entries_out) {
                f(entry);
            }
        }
};

#endif