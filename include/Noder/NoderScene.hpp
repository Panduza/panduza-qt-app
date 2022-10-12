#pragma once

#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <PanduzaEngine.hpp>

class GNode;

class NoderScene : public QGraphicsScene
{
    public:
        NoderScene(PanduzaEngine *engine);
        ~NoderScene();

        GNode *findNodeAt(QPointF pos);
        std::vector<GNode *> findInputEventNodes(void);
        void executeSection(GNode *start);
        void executeScene(void);

    private:
        PanduzaEngine *_engine;
};