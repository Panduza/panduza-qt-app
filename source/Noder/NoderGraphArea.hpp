#pragma once

#include <PzaSplitter.hpp>
#include <PzaTabWidget.hpp>
#include "NoderOutputArea.hpp"

class NoderGraphArea : public PzaSplitter
{
    public:
        NoderGraphArea(QWidget *parent = nullptr);
        ~NoderGraphArea() = default;

    private:
        PzaTabWidget *_tab;
        NoderOutputArea *_outputArea;
};