#pragma once

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QStackedWidget>

class PzaSpoiler : public QWidget
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

        const QStackedWidget *content(void) {return _content;}

    private:
        bool _fold;
        QStackedWidget *_content;
        QToolButton *_header;
        QVBoxLayout *_layout;

    private slots:
        void headerClicked(void);
};