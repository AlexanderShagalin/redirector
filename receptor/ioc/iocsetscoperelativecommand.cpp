#include "iocsetscoperelativecommand.h"
#include <stdexcept>

IocSetScopeRelativeCommand::IocSetScopeRelativeCommand(IScopePtr current, IScopeChangerPtr changer, string targetScope)
    : m_pCurrent(current), m_pChanger(changer), m_sTargetScope(targetScope)
{
}

void IocSetScopeRelativeCommand::execute()
{
    if (m_pCurrent == nullptr)
        throw std::runtime_error("IocSetScopeRelativeCommand::execute(): current scope is nullptr");

    auto scope = m_pCurrent->FindChild(m_sTargetScope);

    if (scope != nullptr)
    {
        if (m_pChanger == nullptr)
            throw std::runtime_error("IocSetScopeRelativeCommand::execute(): scope changer is nullptr");

        m_pChanger->Change(scope);
    }
    else
        throw std::runtime_error("IocSetScopeRelativeCommand::execute(): wrong scope name");
}
