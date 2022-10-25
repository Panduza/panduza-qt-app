#include "NoderView.hpp"
#include "NoderStyle.hpp"
#include "NoderDataBase.hpp"

#include <GNode.hpp>
#include <Nodes/NInstance.hpp>

NoderGraphicsView::NoderGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
    _clickpos(0, 0)
{
    NoderScene *scene = new NoderScene(this);
    setScene(scene);
    _scene = scene;

    setAcceptDrops(true);

    _style = NoderStyle("DefaultTheme.json");

    setBackgroundBrush(QBrush(_style.backgroundCol()));
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setRenderHint(QPainter::Antialiasing);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::NoDrag);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initViewMenu();
}

void NoderGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
    event->accept();
}

void NoderGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    if (event->mimeData()->hasFormat("noder/variable") == true)
        event->accept();
    else
        event->ignore();
}

void NoderGraphicsView::dropEvent(QDropEvent *event)
{
    const PzaMimeData *mime;
    NoderVariable *variable;

    QGraphicsView::dropEvent(event);
    if (event->mimeData()->hasFormat("noder/variable") == false)
        return ;

    mime = static_cast<const PzaMimeData *>(event->mimeData());
    variable = static_cast<NoderVariable *>(mime->dataPtr());
    if (variable) {
        GNode *node;
        QPointF pos = mapToScene(event->position().toPoint());
        
        node = createNode(GNode::CreateNode<Instance>, pos);
        static_cast<Instance *>(node)->setVariable(variable);
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
        case Qt::Key_Shift:
            setDragMode(QGraphicsView::RubberBandDrag);
            break ;
        case Qt::Key_Delete:
            QList<QGraphicsItem *> items;

            items = _scene->selectedItems();
            for (auto item: items) {
                GNode *node = dynamic_cast<GNode *>(item);
                if (node) {
                    if (_selectedNode == node)
                        _selectedNode = nullptr;
                    nodeRemoved(node);
                }
                _scene->removeItem(item);
                delete item;
            }
            //_scene->executeScene();
            break;
    }
}

void NoderGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);

    switch (event->key()) {
        case Qt::Key_A:
            _scene->executeScene();
            break;
        case Qt::Key_Shift:
            setDragMode(QGraphicsView::ScrollHandDrag);
            break ;
    }
}

void NoderGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsView::contextMenuEvent(event);

    _viewMenu->popup(event->globalPos());
}

#include <QColorDialog>

void NoderGraphicsView::mousePressEvent(QMouseEvent *event)
{
   // QGraphicsView::mousePressEvent(event);

    QColorDialog *lol = new QColorDialog();
    lol->exec();

    _clickpos = mapToScene(event->pos());
}

void NoderGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF diff;

    if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            diff = _clickpos - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(diff.x(), diff.y()));
        }
    }
    else {
        _curpos = mapToScene(event->pos());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void NoderGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void NoderGraphicsView::initViewMenu(void)
{
    PzaLabel *label;
    QWidgetAction *ALabel;
    
    std::vector<PzaMenu *> &menus = NBD_INST.nodeMenuList();
    _viewMenu = new PzaMenu(this);

    label = new PzaLabel("Add node");
    ALabel = new QWidgetAction(label);
    ALabel->setDefaultWidget(label);

    _viewMenu->addAction(ALabel);

    for (auto menu: menus) {
        setViewMenuCallback(menu);
        _viewMenu->addMenu(menu);
    }
}

void NoderGraphicsView::selectNode(GNode *node)
{
    qDebug() << "New node selected" << node->name();
    _selectedNode = node;
    nodeSelected(node);
}

#include <QColorDialog>

GNode *NoderGraphicsView::createNode(const NoderDataBase::t_CreateNode &f, const QPointF &pos)
{
    GNode *node;
    
    node = f();
    node->setScene(_scene);
    node->setPos(pos);
    nodeCreated(node);
    connect(node, &GNode::selected, this, [&, node]() {
        selectNode(node);
    });
    return node;
}

void NoderGraphicsView::setViewMenuCallback(QMenu *menu)
{
    foreach(QAction *action, menu->actions()) {
        if (action->menu()) {
            setViewMenuCallback(action->menu());
        }
        else if (action) {
            connect(action, &QAction::triggered, this, [&]
            {
                QAction *action = static_cast<QAction *>(sender());
                action->data();
                NoderDataBase::t_CreateNode f = action->data().value<NoderDataBase::t_CreateNode>();
                createNode(f, _clickpos);
            });
        }
    }
}

void NoderGraphicsView::showEvent(QShowEvent *event)
{
    scene()->setSceneRect(rect());
    QGraphicsView::showEvent(event);
}

bool NoderGraphicsView::isInEllipse(QRectF &rect, QPointF &point)
{
  double h, k, a, b, x, y;

  h = rect.center().x();
  k = rect.center().y();

  a = rect.size().width() / 2;
  b = rect.size().height() / 2;

  x = point.x();
  y = point.y();

  return ((pow(x - h, 2) / pow(a, 2) + pow(y - k, 2) / pow(b, 2)) <= 1.0);
}

double inline NoderGraphicsView::calcRadius(double a, double b, double theta)
{
  return (1 / (sqrt(pow(cos(theta), 2) / pow(a, 2) + pow(sin(theta), 2) / pow(b, 2))));
}

double NoderGraphicsView::ellispeDistance(QRectF &rect, QPointF &point)
{
    double h, k, a, b, x, y;
    QPointF dist;
    double theta, thetamax;
    double r, rmax;
    QPointF border, maxborder;
    QLineF line;
    QLineF linemax;
    double perc;

    h = rect.center().x();
    k = rect.center().y();  
    
    a = rect.size().width() / 2;
    b = rect.size().height() / 2;   
    
    x = point.x();
    y = point.y();

    dist = QPointF(x - h, y - k);

    theta = qAtan(dist.y() / dist.x());
    thetamax = qAtan((rect.bottomRight().y() - k) / (rect.bottomRight().x() - h));

    r = calcRadius(a, b, theta);
    rmax = calcRadius(a, b, thetamax);

    border = QPointF(h + r * cos(theta), k + r * sin(theta));
    maxborder = QPointF(h + rmax * cos(thetamax),k + rmax * sin(thetamax));

    line = QLineF(border, point);
    linemax = QLineF(maxborder, rect.bottomRight());

    perc = 100.0 - std::clamp(line.length() * 100.0 / linemax.length(), 0.0, 100.0);

    return perc;
}

QColor NoderGraphicsView::distanceColor(double distance)
{
    QColor color;

    color = _style.gridCol();
    color.setAlphaF(distance / 100.0 * _style.gridCol().alphaF());
    return color;
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
    double distance;
    double realcenterx;
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
    realcenterx = (realbr.x() + realtl.x()) / 2.0;

    gridtl = QPoint(std::floor(realtl.x() / gridStep - 1), std::floor(realtl.y() / gridStep - 1));
    gridbr = QPoint(std::floor(realbr.x() / gridStep + 1), std::floor(realbr.y() / gridStep + 1));
    ellispe = QRectF(realtl, realbr);
    pen.setWidth(1.0 / transform().m11());

    for (int y = gridtl.y(); y <= gridbr.y(); y++) {
        for (int x = gridtl.x(); x <= gridbr.x(); x++) {
            dot = QPoint(x * gridStep, y * gridStep);
            if (!isInEllipse(ellispe, dot)) {
                mirror = dot;
                if (mirror.x() < realcenterx)
                    mirror.setX(realcenterx + (realcenterx - mirror.x()));
                distance = ellispeDistance(ellispe, mirror);
                pen.setColor(distanceColor(distance));
            }
            else {
                pen.setColor(_style.gridCol());
            }
            painter->setPen(pen);
            painter->drawPoint(dot);
        }
    }
}

NoderNodeArea::NoderNodeArea(QWidget *parent)
    : PzaSpoiler("Node properties", parent)
{

}


void NoderNodeArea::addNode(GNode *node)
{
    addWidget(node->propTable());
}

void NoderNodeArea::removedNode(GNode *node)
{
    removeWidget(node->propTable());
}

void NoderNodeArea::setCurrentNode(GNode *node)
{
    setCurrentWidget(node->propTable());
}

#include <QColorDialog>

NoderViewPanel::NoderViewPanel(NoderGraphicsView *view, QWidget *parent)
    : PzaScrollArea(parent)
{
    _main = new PzaWidget(this);
    _layout = new QVBoxLayout(_main);
    _nodeArea = new NoderNodeArea(_main);

    _main->setStyleSheet("background-color: #252525");

    connect(view, &NoderGraphicsView::nodeCreated, _nodeArea, &NoderNodeArea::addNode);
    connect(view, &NoderGraphicsView::nodeRemoved, _nodeArea, &NoderNodeArea::removedNode);
    connect(view, &NoderGraphicsView::nodeSelected, _nodeArea, &NoderNodeArea::setCurrentNode);

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