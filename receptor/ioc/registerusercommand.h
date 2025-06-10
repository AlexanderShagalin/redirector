#ifndef REGISTERUSERCOMMAND_H
#define REGISTERUSERCOMMAND_H

#include <icommand.h>
#include <string>

class RegisterUserCommand : public ICommand
{
    std::string m_username;
public:
    RegisterUserCommand(std::string username);

    // ICommand interface
public:
    void execute();
};

#endif // REGISTERUSERCOMMAND_H
