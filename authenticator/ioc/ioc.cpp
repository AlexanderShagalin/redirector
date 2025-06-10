#include <ioc.h>
#include <resolvercollection.h>

IResolverCollectionPtr IoC::Resolvers = std::make_shared<ResolverCollection>();
