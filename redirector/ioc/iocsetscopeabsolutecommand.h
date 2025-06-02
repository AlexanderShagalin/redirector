#ifndef IOCSETSCOPEABSOLUTECOMMAND_H
#define IOCSETSCOPEABSOLUTECOMMAND_H

#include <icommand.h>
#include <iscope.h>
#include <iscopechanger.h>
#include <memory>
#include <string>

class IocSetScopeAbsoluteCommand: public ICommand
{
    IScopePtr m_pRoot;
    IScopeChangerPtr m_pChanger;
    std::string m_sTargetScope;

public:
    IocSetScopeAbsoluteCommand(IScopePtr root, IScopeChangerPtr changer, std::string targetScope);

    void execute() override;
};

using IocSetScopeAbsoluteCommandPtr = std::shared_ptr<IocSetScopeAbsoluteCommand>;

#endif
