// Panduza
#include "PzaAdminButton.hpp"
#include <Store.hpp>

// Qt
#include <QLabel>
#include <QHBoxLayout>

// ============================================================================
// 
PzaAdminButton::PzaAdminButton(const QString &text, QWidget *parent)
    : QWidget(parent)
    , _stack(new QStackedWidget(this))
    , _button(new QPushButton(text, this))
{
    initialize();
}

// ============================================================================
// 
PzaAdminButton::PzaAdminButton(QWidget *parent)
    : QWidget(parent)
    , _stack(new QStackedWidget(this))
    , _button(new QPushButton(this))
{
    initialize();
}

// ============================================================================
// 
void PzaAdminButton::initialize()
{
    // Build the stack
    _stack->addWidget( produce_mask_widget() );
    _stack->addWidget(_button);

    // Set the stack as main widget
    auto lay_0 = new QHBoxLayout(this);
    lay_0->addWidget(_stack);

    // Get the size policy from the button
    setSizePolicy(_button->sizePolicy());

    // Display admin view only if the admin connection is ready
    auto display_admin_view_if_admin_conn_ready = [this](){
        if( Store::Get().connection.admin.isReady() )
        {
            _stack->setCurrentIndex(1);
        }
        else
        {
            _stack->setCurrentIndex(0);
        }
    };
    connect(&Store::Get().connection.admin, &AdminConnection::statusChanged, display_admin_view_if_admin_conn_ready);
    display_admin_view_if_admin_conn_ready();
}

// ============================================================================
// 
QWidget* PzaAdminButton::produce_mask_widget()
{
    auto mask_widget = new QPushButton(QIcon(":/image/cyborg-face"), "Only For Admin", this);
    mask_widget->setEnabled(false);
    mask_widget->setIconSize(QSize(40,40));
    mask_widget->setObjectName("admin_button_mask");
    return mask_widget;
}
