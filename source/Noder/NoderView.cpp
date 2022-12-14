#include "NoderView.hpp"
#include "Noder.hpp"
#include "NoderFrame.hpp"

#include <GNode.hpp>
#include <Nodes/NFInstance.hpp>
#include <Nodes/NFMath.hpp>
#include <QGuiApplication>

NoderGraphicsView::NoderGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
    _clickpos(0, 0)
{
    NoderScene *scene = new NoderScene(this);
    setScene(scene);
    _scene = scene;

    setAcceptDrops(true);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setRenderHint(QPainter::Antialiasing);
    setCacheMode(QGraphicsView::CacheBackground);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::NoDrag);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void NoderGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
    event->accept();
}

void NoderGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    if (event->mimeData()->hasFormat("application/json"))
        event->accept();
    else
        event->ignore();
}

void NoderGraphicsView::dropEvent(QDropEvent *event)
{
    const QMimeData *mime;
    QJsonDocument doc;
    QJsonObject obj;

    QGraphicsView::dropEvent(event);
//    if (event->mimeData()->hasFormat("noder/function") == true) {
//        NoderFunction *function;
//
//        mime = static_cast<const PzaMimeData *>(event->mimeData());
//        function = static_cast<NoderFunction *>(mime->dataPtr());
//        if (function && function->view() != this) {
//            QPointF pos = mapToScene(event->position().toPoint());
//            GNode *node = createNode(GNode::CreateNode<FuncInstance>, pos);
//            static_cast<FuncInstance *>(node)->setFunction(function);
//        }
//    }
    if (event->mimeData()->hasFormat("application/json") == true) {
        mime = event->mimeData();
        doc = QJsonDocument::fromJson(mime->data("application/json"));
        obj = doc.object();
        const QString &type = obj["Object Type"].toString();
        const QString &name = obj["Name"].toString();
        if (type == "Variable") {
            NoderSPVariableEntry *variable;

            if (obj["Global"].toBool() == true) {
                variable = Noder::Get().Frame->SidePanel.VariableArea->findVariableEntry(name);
                GNode *node;
                if (variable) {
                    //if (QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::ControlModifier) {
                    //    qDebug() << "Mdr";
                    //}
                    node = createNode(GNode::CreateNode<VariableInstance>, mapToScene(event->position().toPoint()));
                    static_cast<VariableInstance *>(node)->setVariableEntry(variable);
                }
            }
        }
        else if (type == "Function") {

        }
    }
}

void NoderGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint delta;
    int direction;
    double m11;

    QGraphicsView::wheelEvent(event);

    delta = event->angleDelta();
    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    direction = delta.y() / std::abs(delta.y());
    m11 = transform().m11();

    if (direction > 0 && m11 < 2.0)
        scale(1.1, 1.1);
    else if (direction < 0 && m11 > 0.1)
        scale(0.9, 0.9);
}

void NoderGraphicsView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    switch (event->key()) {
        case Qt::Key_Delete:
        {
            QList<QGraphicsItem *> items = _scene->selectedItems();
            for (auto const &item: items) {
                GNode *node = dynamic_cast<GNode *>(item);
                if (node && node->isEternal() == false) {
                    if (_selectedNode == node)
                        _selectedNode = nullptr;
                    _scene->removeItem(item);
                    node->kill();
                }
            }
            break;
        }
        case Qt::Key_A:
            _scene->executeScene();
            break;
        case Qt::Key_Shift:
        {
            setDragMode(QGraphicsView::RubberBandDrag);
            break;
        }
        default:
            break;
    }
}

void NoderGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
}

void NoderGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (_moveCanceled == true)
        _moveCanceled = false;

    _clickpos = mapToScene(event->pos());
    
    QGraphicsView::mousePressEvent(event);
}

void NoderGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    if (_moveCanceled == false
        && scene()->mouseGrabberItem() == nullptr
        && event->buttons() == Qt::LeftButton)
    {
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            if (dragMode() != QGraphicsView::ScrollHandDrag)
                setDragMode(QGraphicsView::ScrollHandDrag);
            QPointF diff = _clickpos - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(diff.x(), diff.y()));
        }
        else {
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void NoderGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    setDragMode(QGraphicsView::NoDrag);
    
    QGraphicsView::mouseReleaseEvent(event);
}

void NoderGraphicsView::selectNode(GNode *node)
{
    _selectedNode = node;
    nodeSelected(node);
}

GNode *NoderGraphicsView::createNode(const Noder::t_CreateNode &f, const QPointF &pos)
{
    GNode *node;
    
    node = f();
    node->setScene(_scene);
    node->setPos(pos);
    nodeCreated(node);
    connect(node, &GNode::selected, this, [&, node]() {
        selectNode(node);
    });
    connect(node, &GNode::destroyed, this, [&, node]() {
        nodeRemoved(node);
    });
    return node;
}

void NoderGraphicsView::createNodeFromMenu(void)
{
    QAction *action = static_cast<QAction *>(sender());
    const Noder::t_CreateNode &f = action->data().value<Noder::t_CreateNode>();
    createNode(f, _clickpos);
}

void NoderGraphicsView::forEachSelected(const std::function<void(QGraphicsItem *item)> &f)
{
    for (auto item : _scene->selectedItems()) {
        f(item);
    }
}

void NoderGraphicsView::doAction(const QString &action)
{
    static std::map<QString, lol> mapMdr = {
        {"move", &NoderGraphicsView::moveSelected},
        {"delete", &NoderGraphicsView::deleteSelected}
    };
    
    (this->*mapMdr[action])();
}

void NoderGraphicsView::moveSelected(void)
{
    qDebug() << "Move not supported for now";
}

void NoderGraphicsView::deleteSelected(void)
{
    forEachSelected([&](QGraphicsItem *item) {
        if (dynamic_cast<GNode *>(item)) {
            deleteNode((GNode *)item);
        }
        else {
            _scene->removeItem(item);
            delete item;
        }
    });
}

void NoderGraphicsView::deleteNode(GNode *node)
{
    if (node->isEternal() == false) {
        _scene->removeItem(node);
        node->kill();
    }
}

void NoderGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsView::contextMenuEvent(event);

    if (_viewMenu)
        _viewMenu->popup(event->globalPos());
}

void NoderGraphicsView::showEvent(QShowEvent *event)
{
    scene()->setSceneRect(rect());
    QGraphicsView::showEvent(event);
}

void NoderGraphicsView::drawBackground(QPainter *painter, const QRectF &r)
{
    QGraphicsView::drawBackground(painter, r);

    QPen pen;
    QPointF realtl, realbr;
    QPoint gridtl, gridbr;
    QPointF dot;
    QRectF ellispe;
    int gridStep;
    QPointF mirror;

    double scale = transform().m11();

    gridStep = 10;
    if (scale < 2.853)
        gridStep *= 2;
    if (scale < 1.21)
        gridStep *= 2;
    if (scale < 0.478)
        gridStep *= 2;

    gridStep = 10 / transform().m11();

    realtl = mapToScene(rect().topLeft());
    realbr = mapToScene(rect().bottomRight());

    gridtl = QPoint(std::floor(realtl.x() / gridStep - 1), std::floor(realtl.y() / gridStep - 1));
    gridbr = QPoint(std::floor(realbr.x() / gridStep + 1), std::floor(realbr.y() / gridStep + 1));
    ellispe = QRectF(realtl, realbr);
    pen.setWidth(1.0 / transform().m11());

    for (int y = gridtl.y(); y <= gridbr.y(); y++) {
        for (int x = gridtl.x(); x <= gridbr.x(); x++) {
            dot = QPoint(x * gridStep, y * gridStep);
            painter->setPen(pen);
            painter->drawPoint(dot);
        }
    }
}

void NoderGraphicsView::addNodeToMenu(PzaMenu *toMenu, const QString &name, const Noder::t_CreateNode &f)
{
    QAction *action;
    
    action = new QAction(name, toMenu);
    action->setData(QVariant::fromValue(f));
    toMenu->addAction(action);
    connect(action, &QAction::triggered, this, &NoderGraphicsView::createNodeFromMenu);

}

NoderViewPanel::NoderViewPanel(NoderGraphicsView *view, QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    _nodeArea = new PzaSpoiler("Node Properties", this);

    connect(view, &NoderGraphicsView::nodeCreated, _nodeArea, [&](const GNode *node) {
        _nodeArea->addWidget(node->propTable());
    });
    connect(view, &NoderGraphicsView::nodeRemoved, _nodeArea, [&](const GNode *node) {
        _nodeArea->removeWidget(node->propTable());
    });
    connect(view, &NoderGraphicsView::nodeSelected, _nodeArea, [&](const GNode *node) {
        _nodeArea->setCurrentWidget(node->propTable());
    });

    _layout->addWidget(_nodeArea);
    _layout->addStretch(1);
    setWidget(_main);
}

NoderView::NoderView(QWidget *parent)
    : PzaSplitter(parent)
{
    _view = new NoderGraphicsView(this);
    _viewPanel = new NoderViewPanel(_view, this);

    setStretchFactor(0, 1);

    addWidget(_view);
    addWidget(_viewPanel);
}
