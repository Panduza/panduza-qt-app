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
#include "NoderSPVariable.hpp"

class NoderGraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        NoderGraphicsView(QWidget *parent = nullptr);

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

        void forEachSelected(const std::function<void(QGraphicsItem *item)> &f);

        void setMoveCanceled(bool state) {_moveCanceled = state;}
        void setMenu(PzaMenu *menu) {_viewMenu = menu;}
        void addNodeToMenu(PzaMenu *toMenu, const QString &name, const Noder::t_CreateNode &f);
        GNode *createNode(const Noder::t_CreateNode &f, const QPointF &pos = QPointF(0, 0));
        void deleteNode(GNode *node);
        void moveSelected(void);
        void deleteSelected(void);

        typedef void (NoderGraphicsView::*lol)();

        void doAction(const QString &action);

    private:
        QPointF _clickpos;
        NoderScene *_scene = nullptr;
        NoderStyle _style;
        GNode *_selectedNode = nullptr;
        bool _moveCanceled = false;
        PzaMenu *_viewMenu = nullptr;


        void selectNode(GNode *node);
        void setViewMenuCallback(QMenu *menu);

    public slots:
        void createNodeFromMenu(void);

    signals:
        void nodeSelected(GNode *);
        void nodeCreated(GNode *);
        void nodeRemoved(GNode *);
};

class NoderViewPanel : public PzaScrollArea
{
    public:
        NoderViewPanel(NoderGraphicsView *view, QWidget *parent = nullptr);

    private:
        PzaWidget *_main;
        QVBoxLayout *_layout;
        PzaSpoiler *_nodeArea;
};

class NoderView : public PzaSplitter
{
    public:
        NoderView(QWidget *parent = nullptr);

        NoderGraphicsView *view() const {return _view;}

    private:
        NoderGraphicsView *_view;
        NoderViewPanel *_viewPanel;
        QHBoxLayout *_layout;
};