#pragma once

#include <QDebug>
#include <QVBoxLayout>

#include <PzaPushButton.hpp>
#include <PzaLabel.hpp>
#include <PzaWidget.hpp>

class PzaMoreLess : public PzaWidget
{
    Q_OBJECT

    public:
        PzaMoreLess(QWidget *parent = nullptr);
        PzaMoreLess(const QString &name, QWidget *parent = nullptr);

    private:
        QHBoxLayout *_layout;
        PzaLabel *_name;
        PzaPushButton *_more;
        PzaPushButton *_less;

    signals:
        void more(void);
        void less(void);
};