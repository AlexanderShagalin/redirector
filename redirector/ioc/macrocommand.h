#ifndef MACROCOMMAND_H
#define MACROCOMMAND_H

#include <list>
#include "icommand.h"

class MacroCommand : public ICommand
{
    std::list<ICommandPtr> m_commands;

public:
    MacroCommand(std::list<ICommandPtr> commands);
    void execute() override;
};

#endif // MACROCOMMAND_H
