#include "NoderGraphArea.hpp"

NoderGraphArea::NoderGraphArea(QWidget *parent)
    : PzaSplitter(Qt::Vertical, parent)
{
    _tab = new PzaTabWidget(this);
    _outputArea = new NoderOutputArea(this);

    setStretchFactor(0, 1);

    addWidget(_tab);
    addWidget(_outputArea);
}