#ifndef SCOPEHOLDER_H
#define SCOPEHOLDER_H

#include <iresolvercollection.h>
#include <iscope.h>
#include <scopecontainer.h>


class ResolverCollection: public IResolverCollection
{
    IScopePtr m_pRootScope;
    thread_local static ScopeContainerPtr s_pCurrentScopeContainer;
    
    void Register(string dependecy, IResolverContainerPtr resolver);
    ScopeContainerPtr getCurrentScopeContainer();

public:
    ResolverCollection();
    
    IResolverContainerPtr findContainer(string dependecy) override;
};

#endif
