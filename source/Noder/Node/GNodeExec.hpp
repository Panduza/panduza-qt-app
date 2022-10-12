#pragma once

#include <GNode.hpp>

class GNodeExec : public GNode
{
    protected:
        GNodeExec(const QString &name, bool in, bool out);
        GNodeExec(const QString &name);
        ~GNodeExec() = default;

        GNode *branch(void) override;
        void evaluateOperations(GNode *node);
    
    private:
        PinDecl::Exec *_execIn = nullptr;
        PinDecl::Exec *_execOut = nullptr;
};