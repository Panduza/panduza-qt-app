#pragma once

#include <QSplitter>
#include <PzaWidget.hpp>

class PzaSplitter : public QSplitter
{
    public:
        PzaSplitter(QWidget *parent = nullptr);
        PzaSplitter(Qt::Orientation, QWidget* parent = nullptr);
        ~PzaSplitter() = default;
};