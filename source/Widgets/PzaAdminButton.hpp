#pragma once

// Qt
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>

/**
 * @brief Button that can hide itself when admin connection is invalid
 */
class PzaAdminButton : public QWidget
{
    public:

        // PzaAdminButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
        PzaAdminButton(const QString &text, QWidget *parent = nullptr);
        PzaAdminButton(QWidget *parent = nullptr);

        /**
         * @brief Easy way to get the internal button for event connection
         * 
         * Be carreful using this, when you grab it your not the owner
         * 
         * @return QPushButton* 
         */
        QPushButton* internalComponent() { return _button; }

    protected:

        /**
         * @brief Build the widget
         */
        void initialize();

        /**
         * @brief Build the widget that mask the button
         * 
         * @return QWidget* 
         */
        QWidget* produce_mask_widget();

        /**
         * @brief Stack used to hide the button when admin connection is invalid
         */
        QStackedWidget* _stack;

        /**
         * @brief Actual button
         */
        QPushButton* _button;

};
