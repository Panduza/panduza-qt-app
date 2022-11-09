#include "PzaTreeWidget.hpp"

PzaTreeWidget::PzaTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setStyleSheet(
        "QTreeView {"
        "   background-color: #1e1e1e;"
        "}"
        ""
        "QTreeView::item {"
        "   color: #DCDCDC;"
        "   border-top-color: transparent;"
        "   border-bottom-color: transparent;"
        "}"
        ""
        "QTreeView::item:hover {"
        "   background-color: #374F6B;"
        "}"
    );

    verticalScrollBar()->hide();
}