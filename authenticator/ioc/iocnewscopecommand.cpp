#include <stdexcept>

#include "iocnewscopecommand.h"
#include "scope.h"

IocNewScopeCommand::IocNewScopeCommand(IScopePtr pCurrentScope, string name)
    : m_pCurrentScope(pCurrentScope), m_sName(name)
{
}

void IocNewScopeCommand::execute()
{
    if (m_pCurrentScope == nullptr)
        throw std::runtime_error("IocNewScopeCommand: m_pCurrentScope is nullptr");

    if (m_pCurrentScope->FindChild(m_sName) != nullptr)
        throw std::runtime_error(std::string("IocNewScopeCommand: scope \"") + m_sName + std::string("\" already exist"));

    m_pCurrentScope->AddChild( std::make_shared<Scope>(m_sName, m_pCurrentScope) );
}
