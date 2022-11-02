#pragma once

#include <GNodeBasic.hpp>

enum class NodeExecDirection {
    Input,
    Output,
    Bidirectionnal
};

class GNodeExec : public GNodeBasic
{
    protected:
        GNodeExec(const QString &name, NodeExecDirection direction);
        GNodeExec(const QString &name);

        GNode *branch(void) override;
        void evaluateOperations(GNode *node);
    
    private:
        PinDecl::Exec *_execIn = nullptr;
        PinDecl::Exec *_execOut = nullptr;
};