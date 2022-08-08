#ifndef NODERVIEW_HPP
#define NODERVIEW_HPP

#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <QWidgetAction>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHeaderView>

#include <NoderStyle.hpp>
#include <NoderScene.hpp>
#include <NoderDataModel.hpp>
#include <PzaMenu.hpp>

class NoderView : public QGraphicsView
{
    public:
        NoderView(PanduzaEngine *engine, NoderDataModel *model);
        ~NoderView() = default;

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

    private:
        PanduzaEngine *_engine;
        NoderDataModel *_model;
        NoderScene *_scene;
        NoderStyle _style;
        QPointF _clickpos;
        QPointF _curpos;
        PzaMenu *_viewMenu;

        void initViewMenu(void);
        void setViewMenuCallback(QMenu *menu);
        bool isInEllipse(QRectF &rect, QPointF &point);
        double inline calcRadius(double a, double b, double theta);
        double ellispeDistance(QRectF &rect, QPointF &point);
        QColor distanceColor(double distance);
};

#endif