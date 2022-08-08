#include <NoderScene.hpp>
#include <NodeEvent.hpp>

NoderScene::NoderScene(PanduzaEngine *engine)
    : QGraphicsScene(),
    _engine(engine)
{

}

Node *NoderScene::findNodeAt(QPointF pos)
{
    QList<QGraphicsItem *> list;
    Node *node = nullptr;

    list = items(pos);
    for (auto elem: list) {
        node = dynamic_cast<Node *>(elem);
        if (node != nullptr)
            break;
    }
    return node;
}

std::vector<Node *> NoderScene::findInputEventNodes(void)
{
    std::vector<Node *> list;
    Node *node;

    for (auto elem: items()) {
        node = dynamic_cast<NodeEvent *>(elem);
        if (node != nullptr) {
            list.push_back(node);
        }
    }
    return list;
}

void NoderScene::executeSection(Node *start)
{
    Node *node;

    node = start;
    while (node)
        node = node->branch();
}

void NoderScene::executeScene(void)
{
    std::vector<Node *> inputs;

    _engine->showTree();

    inputs = findInputEventNodes();

    for (auto input: inputs) {
        executeSection(input);
    }
}

NoderScene::~NoderScene()
{
    
}
