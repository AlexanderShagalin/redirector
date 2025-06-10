#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <functional>
#include <list>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <dlfcn.h>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/json.hpp>


#include "icommand.h"
#include "ioc.h"
#include "serverprocesscommand.h"
#include "httpredirector.h"
#include "registerusercommand.h"
#include "checkusercommand.h"


using namespace std;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

static volatile sig_atomic_t flag = 0;
static std::pair<std::string, std::string> authdata;

void signal_handler(int)
{
    IoC::Resolve<void>("Server.Shutdown.Set", 1);
    std::cout << "Ctrl+C handled" << std::endl;
}

void createEnvironment()
{
    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Server.Shutdown.Get",
        make_container(std::function<int()>(
            []()
            {
                return flag;
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Server.Shutdown.Set",
        make_container(std::function<void(int)>(
            [](int value)
            {
                flag = value;
            } ))
        )->execute();


    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "ServerProcessCommand",
        make_container(std::function<ICommandPtr()>(
            []()
            {
                return std::dynamic_pointer_cast<ICommand>(std::make_shared<ServerProcessCommand>());
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Redirector.Get",
        make_container(std::function<IRedirectorPtr()>(
            []()
            {
                return std::dynamic_pointer_cast<IRedirector>(std::make_shared<HttpRedirector>());
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "RegisterUserCommand",
        make_container(std::function<ICommandPtr(std::string)>(
            [](std::string username)
            {
                return std::dynamic_pointer_cast<ICommand>(std::make_shared<RegisterUserCommand>(username));
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "CheckUserCommand",
        make_container(std::function<ICommandPtr(std::string, std::string)>(
            [](std::string token, std::string username)
            {
                return std::dynamic_pointer_cast<ICommand>(std::make_shared<CheckUserCommand>(token, username));
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Server.Auth.Set",
        make_container(std::function<void(std::string, std::string)>(
            [](std::string t, std::string n)
            {
                std::cout << "Auth params: " << n << " " << t << std::endl;
                authdata = std::make_pair(t, n);
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Server.Auth.Get",
        make_container(std::function<std::pair<std::string,std::string>(void)>(
            []()
            {
                return authdata;
            } ))
        )->execute();
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT, &sa, 0) == -1)
    {
        std::cout << "Error install signal action" << std::endl;
        return EXIT_FAILURE;
    }

    createEnvironment();

    IoC::Resolve<ICommandPtr>("ServerProcessCommand")->execute();

    return EXIT_SUCCESS;
}
