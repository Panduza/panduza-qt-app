#include "PzaSpoiler.hpp"

PzaSpoiler::PzaSpoiler(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    _layout = new QVBoxLayout(this);
    _header = new QToolButton(this);

    setStyleSheet(
        "QToolButton {"
            "border: none;"
            "background-color:pink;"
        "}"
    );

    _layout->setContentsMargins(0, 0, 0, 0);

    _header->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _header->setArrowType(Qt::ArrowType::RightArrow);
    _header->setCheckable(true);
    _header->setChecked(false);
    _header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    _layout->addWidget(_header);

    connect(_header, &QToolButton::clicked, this, &PzaSpoiler::headerClicked);
    setFold(true);
}

PzaSpoiler::PzaSpoiler(const QString &name, QWidget *parent)
    : PzaSpoiler(parent)
{
    _header->setText(name);
}

void PzaSpoiler::setContentWidget(QWidget *w)
{
    if (_content)
        _layout->removeWidget(_content);
    _content = w;
    _layout->addWidget(_content);
    setFold(_fold);
}

void PzaSpoiler::updateSpoiler()
{
    if (_content == nullptr)
        return ;
    if (_fold) {
        _content->setVisible(false);
    }
    else {
        _content->setVisible(true);
    }
    if (_fold)
        _header->setArrowType(Qt::ArrowType::RightArrow);
    else
        _header->setArrowType(Qt::ArrowType::DownArrow);
}

void PzaSpoiler::headerClicked(void)
{
    setFold(!_fold);
}

void PzaSpoiler::setFold(bool state)
{
    _fold = state;

    updateSpoiler();
}