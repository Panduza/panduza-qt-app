#include <NoderScene.hpp>
#include <GNode.hpp>
#include <GNodeEvent.hpp>

NoderScene::NoderScene(QWidget *parent)
    : QGraphicsScene(parent)
{

}

GNode *NoderScene::findNodeAt(QPointF pos)
{
    QList<QGraphicsItem *> list;
    GNode *node = nullptr;

    list = items(pos);
    for (auto elem: list) {
        node = dynamic_cast<GNode *>(elem);
        if (node != nullptr)
            break;
    }
    return node;
}

std::vector<GNode *> NoderScene::findInputEventNodes(void)
{
    std::vector<GNode *> list;
    GNode *node;

    for (auto elem: items()) {
        node = dynamic_cast<GNodeEventStart *>(elem);
        if (node != nullptr) {
            list.push_back(node);
        }
    }
    return list;
}

void NoderScene::executeSection(GNode *start)
{
    GNode *node;

    node = start;
    while (node) {
        node->process();
        node = node->branch();
    }
}

void NoderScene::executeScene(void)
{
    std::vector<GNode *> inputs;

    inputs = findInputEventNodes();

    for (auto input: inputs) {
        executeSection(input);
    }
}

NoderScene::~NoderScene()
{
    
}