#ifndef SENDNORULECOMMAND_H
#define SENDNORULECOMMAND_H

#include <netinet/in.h>

#include <icommand.h>

class SendNoRuleCommand : public ICommand
{
    sockaddr_in m_client;
public:
    SendNoRuleCommand(sockaddr_in client);

    // ICommand interface
public:
    void execute();
};

#endif // SENDNORULECOMMAND_H
