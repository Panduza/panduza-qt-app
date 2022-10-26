#pragma once

#include <QDebug>
#include <PzaWidget.hpp>
#include <QVBoxLayout>
#include <QToolButton>
#include <PzaStackedWidget.hpp>

class PzaSpoiler : public PzaWidget
{
    public:
        PzaSpoiler(QWidget *parent = nullptr);
        PzaSpoiler(const QString &name, QWidget *parent = nullptr);
        ~PzaSpoiler() = default;
    
        void setFold(bool state);
        bool folded(void) const {return _fold;}
        void updateSpoiler(void);
        void addWidget(QWidget *w);
        void removeWidget(QWidget *w);
        void setCurrentWidget(QWidget *w);

        const PzaStackedWidget *content(void) {return _content;}

    private:
        bool _fold;
        PzaStackedWidget *_content;
        QToolButton *_header;
        QVBoxLayout *_layout;

    private slots:
        void headerClicked(void);
};