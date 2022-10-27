#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <QWidgetAction>
#include <QTreeWidget>
#include <QHeaderView>

#include "NoderStyle.hpp"
#include "NoderScene.hpp"
#include <PzaMenu.hpp>
#include <PzaSplitter.hpp>
#include <PzaMimeData.hpp>
#include <PzaScrollArea.hpp>
#include <PzaSpoiler.hpp>
#include "NoderVariable.hpp"

class NoderGraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        NoderGraphicsView(QWidget *parent = nullptr);
        ~NoderGraphicsView() = default;

        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void showEvent(QShowEvent *event) override;
        void drawBackground(QPainter* painter, const QRectF& r) override;
        void contextMenuEvent(QContextMenuEvent *event) override;
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event);

        void setMoveCanceled(bool state) {_moveCanceled = state;}

    private:
        QPointF _clickpos;
        NoderScene *_scene;
        NoderStyle _style;
        PzaMenu *_viewMenu;
        GNode *_selectedNode = nullptr;
        bool _moveCanceled = false;

        GNode *createNode(const NoderDataBase::t_CreateNode &f, const QPointF &pos = QPointF(0, 0));

        void selectNode(GNode *node);
        void initViewMenu(void);
        void setViewMenuCallback(QMenu *menu);

    signals:
        void nodeSelected(GNode *);
        void nodeCreated(GNode *);
        void nodeRemoved(GNode *);
};

class NoderNodeArea : public PzaSpoiler
{
    Q_OBJECT

    public:
        NoderNodeArea(QWidget *parent = nullptr);
        ~NoderNodeArea() = default;

    public slots:
        void addNode(GNode *node);
        void removedNode(GNode *node);
        void setCurrentNode(GNode *node);
};

class NoderViewPanel : public PzaScrollArea
{
    public:
        NoderViewPanel(NoderGraphicsView *view, QWidget *parent = nullptr);
        ~NoderViewPanel() = default;

    private:
        PzaWidget *_main;
        QVBoxLayout *_layout;
        NoderNodeArea *_nodeArea;
};

class NoderView : public PzaSplitter
{
    public:
        NoderView(QWidget *parent = nullptr);
        ~NoderView() = default;

    private:
        NoderGraphicsView *_view;
        NoderViewPanel *_viewPanel;
        QHBoxLayout *_layout;
};