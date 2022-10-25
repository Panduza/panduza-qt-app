#include "PzaSplitter.hpp"

PzaSplitter::PzaSplitter(Qt::Orientation orientation, QWidget *parent)
    : QSplitter(orientation, parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins(0, 0, 0, 0);
}

PzaSplitter::PzaSplitter(QWidget *parent)
    : PzaSplitter(Qt::Orientation::Horizontal, parent)
{

}