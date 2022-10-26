#pragma once

#include <QStackedWidget>

class PzaStackedWidget : public QStackedWidget
{
    public:
        PzaStackedWidget(QWidget *parent = nullptr);
        ~PzaStackedWidget() = default;

        QSize sizeHint() const override
        {
            if (currentWidget())
                return currentWidget()->sizeHint();
            else
                return QStackedWidget::sizeHint();
        }
        QSize minimumSizeHint() const override
        {
            if (currentWidget())
                return currentWidget()->minimumSizeHint();
            else
                return QStackedWidget::minimumSizeHint();
        }
};