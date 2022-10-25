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
#include "NoderVariable.hpp"

class NoderGraphicsView : public QGraphicsView
{
    public:
        NoderGraphicsView(QWidget *parent = nullptr);
        ~NoderGraphicsView() = default;

    protected:
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

    private:
        QPointF _clickpos;
        NoderScene *_scene;
        NoderStyle _style;
        QPointF _curpos;
        PzaMenu *_viewMenu;

        void initViewMenu(void);
        void setViewMenuCallback(QMenu *menu);
        bool isInEllipse(QRectF &rect, QPointF &point);
        double inline calcRadius(double a, double b, double theta);
        double ellispeDistance(QRectF &rect, QPointF &point);
        QColor distanceColor(double distance);
};

class NoderView : public PzaSplitter
{
    public:
        NoderView(QWidget *parent = nullptr);
        ~NoderView() = default;

    private:
        NoderGraphicsView *_view;
        QHBoxLayout *_layout;
};