#pragma once

#include <QWidget>
#include <QMouseEvent>

class PzaWidget : public QWidget
{
    Q_OBJECT
    
    public:
        PzaWidget(QWidget *parent = nullptr);
        ~PzaWidget() = default;

    void mousePressEvent(QMouseEvent *event) override;

    signals:
        void clicked(void);
};