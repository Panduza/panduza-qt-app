#pragma once

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>

class PzaSpoiler : public QWidget
{
    public:
        PzaSpoiler(QWidget *parent = nullptr);
        PzaSpoiler(const QString &name, QWidget *parent = nullptr);
        ~PzaSpoiler() = default;
    
        void setContentWidget(QWidget *w);
        void setFold(bool state);
        bool folded(void) const {return _fold;}
        void updateSpoiler(void);

    private:
        bool _fold;
        QWidget *_content = nullptr;
        QToolButton *_header;
        QVBoxLayout *_layout;

    private slots:
        void headerClicked(void);
};