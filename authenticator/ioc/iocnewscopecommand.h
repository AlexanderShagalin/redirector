#ifndef IocNewScopeCommand_H
#define IocNewScopeCommand_H

#include "icommand.h"
#include "iscope.h"
#include "memory"
#include "string"

class IocNewScopeCommand: public ICommand
{
    IScopePtr m_pCurrentScope;
    std::string m_sName;
public:
    IocNewScopeCommand(IScopePtr pCurrentScope, std::string name);

    void execute() override;
};

using IocNewScopeCommandPtr = std::shared_ptr<IocNewScopeCommand>;

#endif
