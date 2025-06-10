#include <stdexcept>
#include "iocregistercommand.h"

IocRegisterCommand::IocRegisterCommand(IScopePtr scope, std::string dependecy, IResolverContainerPtr resolver)
    :m_pScope(scope), m_sDependecy(dependecy), m_pResolver(resolver)
{

}

void IocRegisterCommand::execute()
{
    if (m_pScope == nullptr)
        throw std::runtime_error("IocRegisterCommand::execute(): m_pScope is nullptr");

    m_pScope->setResolver(m_sDependecy, m_pResolver);
}
