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
        void updateStartNode(void);

    private:
        QString _name;
        GNode *_startNode;
    
    signals:
        void nameChanged(void);
};