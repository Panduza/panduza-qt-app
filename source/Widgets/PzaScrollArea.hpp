#pragma once

#include <QWidget>
#include <QScrollArea>

class PzaScrollArea : public QScrollArea
{
    Q_OBJECT
    
    public:
        PzaScrollArea(QWidget *parent = nullptr);
};