#pragma once

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

#include <PzaLabel.hpp>
#include <PzaStyle.hpp>

class PzaMoreLess : public QWidget
{
    Q_OBJECT

    public:
        PzaMoreLess(const QString &name = "", PzaStyle::Stretch stretch = PzaStyle::Stretch::Left, QWidget *parent = nullptr);
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