#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <functional>
#include <list>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <boost/program_options.hpp>


#include "icommand.h"
#include "irule.h"
#include "rulesequenceloader.h"
#include "ruleloader.h"
#include "jsonrulebuilder.h"
#include "ioc.h"
#include "macrocommand.h"
#include "ruleredirect.h"
#include "sendnorulecommand.h"
#include "serverprocesscommand.h"

volatile static sig_atomic_t flag = 0;
volatile static sig_atomic_t current_port = 18081;

using namespace std;
// namespace json = boost::json;
namespace opt = boost::program_options;
namespace fs = std::filesystem;

void signal_handler(int)
{
    flag = 1;
    std::cout << "Ctrl+C handled" << std::endl;
}

void createEnvironment()
{
    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Rules.Sequence.Loader",
        make_container(std::function<IRuleSequenceLoaderPtr()>(
            []()
            {
                return std::dynamic_pointer_cast<IRuleSequenceLoader>(std::make_shared<RuleSequenceLoader>());
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Rules.Plugins.Loader",
        make_container(std::function<IRuleLoaderPtr()>(
            []()
            {
                return std::dynamic_pointer_cast<IRuleLoader>(std::make_shared<RuleLoader>(std::string(".")));
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Rules.Redirect",
        make_container(std::function<IRulePtr(std::string, IEndPointPtr)>(
            [](std::string target, IEndPointPtr ep)
            {
                return new RuleRedirect(target, ep);
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "JsonRuleBuilder",
        make_container(std::function<IRuleBuilderPtr(boost::json::value, IEndPointPtr)>(
            [](boost::json::value jrules, IEndPointPtr ep)
            {
                return std::dynamic_pointer_cast<IRuleBuilder>(std::make_shared<JsonRuleBuilder>(jrules, ep));
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "SendNoRuleCommand",
        make_container(std::function<ICommandPtr(sockaddr_in)>(
            [](sockaddr_in client)
            {
                return std::dynamic_pointer_cast<ICommand>(std::make_shared<SendNoRuleCommand>(client));
            } ))
        )->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "MacroCommand",
        make_container(std::function<ICommandPtr(std::list<ICommandPtr>)>(
            [](std::list<ICommandPtr> commands)
            {
                return std::dynamic_pointer_cast<ICommand>(std::make_shared<MacroCommand>(commands));
            } ))
        )->execute();

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
        "Server.Port.Set",
        make_container(std::function<void(int)>(
            [](int port)
            {
                current_port = static_cast<uint16_t>(port);
            } ))
        )->execute();


    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "Server.Port.Get",
        make_container(std::function<int(void)>(
            []()
            {
                return current_port;
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
        return -1;
    }

    createEnvironment();

    try {
        opt::options_description desc("Allowed options");
        desc.add_options()
            ("-p", opt::value<string>(), "Port number");
        opt::variables_map option_port;
        opt::store(opt::parse_command_line(argc, argv, desc), option_port);
        opt::notify(option_port);

        if(option_port.count("-p")) {
            IoC::Resolve<void>("Server.Port.Set", option_port["-p"].as<int>());
        }
    } catch (...) {
        cout << "Programm options error" << endl;
    }

    IoC::Resolve<ICommandPtr>("ServerProcessCommand")->execute();

    return 0;
}
