#ifndef NODERSCENE_HPP
#define NODERSCENE_HPP

#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <PanduzaEngine.hpp>

class Node;

class NoderScene : public QGraphicsScene
{
    public:
    
        NoderScene(PanduzaEngine *engine);
        ~NoderScene();

        PanduzaEngine *engine() {return _engine;}
        Node *findNodeAt(QPointF pos);
        void executeSection(Node *start);

    private:
        std::vector<Node *> findInputEventNodes(void);
        PanduzaEngine *_engine;

    public slots:
        void executeScene(void);
};

#endif