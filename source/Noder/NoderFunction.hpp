#pragma once

#include "NoderView.hpp"

#include <PzaPushButton.hpp>

#include <NFArray.hpp>
#include <NFMath.hpp>
#include <NFString.hpp>
#include <NFFor.hpp>
#include <NFIf.hpp>
#include <NFEvent.hpp>
#include <NFDelay.hpp>
#include <NFIO.hpp>

class NoderFunction : public NoderView
{
    Q_OBJECT
    
    public:
        NoderFunction(QWidget *parent = nullptr);

        void setName(const QString &name) {_name = name; nameChanged();}
        const QString &name(void) const {return _name;}
        GNode *startNode(void) const {return _startNode;}
        GNode *endNode(void) const {return _endNode;}
        void updateEndNode(void);

    private:
        QString _name;
        GNode *_startNode;
        GNode *_endNode;
    
    signals:
        void nameChanged(void);
};