#include "stdio.h"
#include "exceptionmanager.h"

std::map<std::pair<size_t, size_t>, IHandlerPtr> ExceptionManager::m_handlerList;

void ExceptionManager::handle(const std::exception& e, ICommandPtr command) 
{
    auto source = std::make_pair<size_t, size_t>(typeid(*command).hash_code(), typeid(e).hash_code());
    // printf("find handle: %lu %lu\n", typeid(*command).hash_code(), typeid(e).hash_code());

    auto it = m_handlerList.find(source);
    if(it != m_handlerList.end())
    {
        // printf("handle\n");
        it->second->handle(command);
    }
}

void ExceptionManager::registerHandler(ICommandPtr command, const std::exception& e, IHandlerPtr handler)
{
    auto source = std::make_pair<size_t, size_t>(typeid(*command).hash_code(), typeid(e).hash_code());
    
    // printf("registerHandler: %lu %lu\n", typeid(*command).hash_code(), typeid(e).hash_code());
    if(m_handlerList.count(source)== 0)
        m_handlerList.insert({ source, handler });
}

void ExceptionManager::clear()
{
    m_handlerList.clear();
}
