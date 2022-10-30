#pragma once

#include <GNode.hpp>

class GNodeInstance : public GNode
{
    protected:
        GNodeInstance();
    
    private:
        void pinBoxSize(void) override;
        void resizeBoxes(void) override;
        void positionEntries(void) override;
        void setWidgetSize(void) override;
        void drawBoxes(QPainter *painter) override;

        void refreshNode(void) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void paint(QPainter *, QStyleOptionGraphicsItem const *opt = 0, QWidget *w = 0) override;
};