#ifndef IOCREGISTERCOMMAND_H
#define IOCREGISTERCOMMAND_H

#include <icommand.h>
#include <iscope.h>
#include <iresolvercontainer.h>
#include <memory>
#include <string>

class IocRegisterCommand: public ICommand
{
    IScopePtr m_pScope;
    std::string m_sDependecy;
    IResolverContainerPtr m_pResolver;

public:
    IocRegisterCommand( IScopePtr scope, std::string dependecy, IResolverContainerPtr resolver);

    void execute() override;
};

using IocRegisterCommandPtr = std::shared_ptr<IocRegisterCommand>;

#endif // IOCREGISTERCOMMAND_H
