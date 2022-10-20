#pragma once

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

#include <PzaLabel.hpp>
#include <PzaWidget.hpp>

class PzaMoreLess : public PzaWidget
{
    Q_OBJECT

    public:
        PzaMoreLess(QWidget *parent = nullptr);
        PzaMoreLess(const QString &name, QWidget *parent = nullptr);
        ~PzaMoreLess() = default;

    private:
        QHBoxLayout *_layout;
        PzaLabel *_name;
        QPushButton *_more;
        QPushButton *_less;

    signals:
        void more(void);
        void less(void);
};