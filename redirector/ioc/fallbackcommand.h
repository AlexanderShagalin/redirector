#ifndef FALLBACKCOMMAND_H
#define FALLBACKCOMMAND_H

#include <icommand.h>
#include <memory>
#include <vector>

class FallbackCommand : public ICommand
{
    std::vector<ICommandPtr> m_Commands;

public:
    FallbackCommand(const std::vector<ICommandPtr> &commands);

    virtual void execute() override;
};

using FallbackCommandPtr = std::shared_ptr<FallbackCommand>;

#endif
