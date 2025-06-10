#ifndef IOCSETSCOPERELATIVECOMMAND_H
#define IOCSETSCOPERELATIVECOMMAND_H

#include "icommand.h"
#include "iscope.h"
#include "iscopechanger.h"
#include <memory>
#include <string>

class IocSetScopeRelativeCommand: public ICommand
{
    IScopePtr m_pCurrent;
    IScopeChangerPtr m_pChanger;
    std::string m_sTargetScope;

public:
    IocSetScopeRelativeCommand(IScopePtr current, IScopeChangerPtr changer, std::string targetScope);

    void execute() override;
};

using IocSetScopeRelativeCommandPtr = std::shared_ptr<IocSetScopeRelativeCommand>;

#endif // IOCSETSCOPERELATIVECOMMAND_H
