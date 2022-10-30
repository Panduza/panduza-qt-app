#pragma once

#include <QWidget>
#include <QMouseEvent>

class PzaWidget : public QWidget
{
    Q_OBJECT
    
    public:
        PzaWidget(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;

    signals:
        void clicked(void);
};