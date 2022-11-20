#include "PzaSpoiler.hpp"

PzaSpoiler::PzaSpoiler(QWidget *parent)
    : PzaWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    _layout = new QVBoxLayout(this);
    _header = new PzaToolButton(this);
    _content = new PzaStackedWidget(this);

    _layout->setContentsMargins(0, 0, 0, 0);

    _header->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _header->setArrowType(Qt::ArrowType::RightArrow);
    _header->setCheckable(true);
    _header->setChecked(false);
    _header->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    _layout->addWidget(_header);
    _content->setVisible(false);
    _layout->addWidget(_content);

    connect(_header, &QToolButton::clicked, this, &PzaSpoiler::headerClicked);
    setFold(false);
}

PzaSpoiler::PzaSpoiler(const QString &name, QWidget *parent)
    : PzaSpoiler(parent)
{
    _header->setText(name);
}

void PzaSpoiler::addWidget(QWidget *w)
{
    _content->addWidget(w);
    setCurrentWidget(w);
}

void PzaSpoiler::removeWidget(QWidget *w)
{
    _content->removeWidget(w);
}

void PzaSpoiler::removeAllWidgets(void)
{
    for(int i = _content->count(); i >= 0; i--)
    {
        QWidget* widget = _content->widget(i);
        _content->removeWidget(widget);
    }
}

void PzaSpoiler::setCurrentWidget(QWidget *w)
{
    _content->setCurrentWidget(w);
}

void PzaSpoiler::updateSpoiler()
{
    if (_fold) {
        _header->setArrowType(Qt::ArrowType::RightArrow);
        if (_content)
            _content->setVisible(false);
    }
    else {
        _header->setArrowType(Qt::ArrowType::DownArrow);
        if (_content)
            _content->setVisible(true);
    }
    update();
}

void PzaSpoiler::headerClicked(void)
{
    (_fold) ? setFold(false) : setFold(true);
}

void PzaSpoiler::setFold(bool state)
{
    _fold = state;

    updateSpoiler();
}