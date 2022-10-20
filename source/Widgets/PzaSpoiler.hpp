#pragma once

#include <QDebug>
#include <PzaWidget.hpp>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStackedWidget>

class PzaSpoiler : public PzaWidget
{
    public:
        PzaSpoiler(QWidget *parent = nullptr);
        PzaSpoiler(const QString &name, QWidget *parent = nullptr);
        ~PzaSpoiler() = default;
    
        void setFold(bool state);
        bool folded(void) const {return _fold;}
        void updateSpoiler(void);
        void addWidget(PzaWidget *w);
        void removeWidget(PzaWidget *w);
        void setCurrentWidget(PzaWidget *w);

        const QStackedWidget *content(void) {return _content;}

    private:
        bool _fold;
        QStackedWidget *_content;
        QToolButton *_header;
        QVBoxLayout *_layout;

    private slots:
        void headerClicked(void);
};