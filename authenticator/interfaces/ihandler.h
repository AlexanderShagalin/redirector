#ifndef IHANDLER_H
#define IHANDLER_H

#include <memory>
#include "icommand.h"

class IHandler
{
public:
    virtual ~IHandler(){}
    virtual void handle(ICommandPtr command) = 0;
};

typedef std::shared_ptr<IHandler> IHandlerPtr;
#endif // IHANDLER_H