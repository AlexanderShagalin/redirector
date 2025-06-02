#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <memory>

class ICommand
{
public:
    virtual ~ICommand(){}
    virtual void execute() = 0;
};

typedef std::shared_ptr<ICommand> ICommandPtr;
#endif // ICOMMAND_H