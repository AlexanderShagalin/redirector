#ifndef CHECKUSERCOMMAND_H
#define CHECKUSERCOMMAND_H

#include <icommand.h>
#include <string>

class CheckUserCommand : public ICommand
{
    std::string m_token;
    std::string m_username;
public:
    CheckUserCommand(std::string token, std::string username);

    // ICommand interface
public:
    void execute();
};

#endif // CHECKUSERCOMMAND_H
