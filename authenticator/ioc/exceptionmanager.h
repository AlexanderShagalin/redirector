#ifndef EXCEPTIONMANAGER_H
#define EXCEPTIONMANAGER_H

#include <exception>
#include <memory>
#include <utility>
#include <map>
#include <list>
#include "icommand.h"
#include "ihandler.h"

class ExceptionManager
{
    static std::map<std::pair<size_t, size_t>, IHandlerPtr> m_handlerList;
public:
   void handle(const std::exception& e, ICommandPtr command);

    static void registerHandler(ICommandPtr command, const std::exception& e, IHandlerPtr handler);
    static void clear();
};

typedef std::shared_ptr<ExceptionManager> ExceptionManagerPtr;
#endif // EXCEPTIONMANAGER_H
