#pragma once

#include <GNodeBasic.hpp>

class GNodeExec : public GNodeBasic
{
    protected:
        GNodeExec(const QString &name, bool in, bool out);
        GNodeExec(const QString &name);

        GNode *branch(void) override;
        void evaluateOperations(GNode *node);
    
    private:
        PinDecl::Exec *_execIn = nullptr;
        PinDecl::Exec *_execOut = nullptr;
};