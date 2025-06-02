#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <list>

#include "serverprocesscommand.h"
#include "ioc.h"
#include "irulebuilder.h"
#include "endpoint.h"
#include "jsonendpoint.h"
#include "irulesequenceloader.h"

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

extern void createEnvironment();

ServerProcessCommand::ServerProcessCommand() : ICommand() {}

void ServerProcessCommand::do_session(tcp::socket& socket)
{
    json::error_code ec;

    std::cout << "Create session" << std::endl;

    IEndPointPtr ep = std::make_shared<JsonEndPoint>(std::make_shared<EndPoint>(socket));

    for(;;)
    {
        std::string strBuf;
        try
        {
            strBuf = ep->read();
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
            break;
        }

        auto responce = json::parse(strBuf, ec);
        if(ec.value() != 0)
        {
            continue;
        }
        auto loader = IoC::Resolve<IRuleSequenceLoaderPtr>("Rules.Sequence.Loader");
        auto loadedRules = loader->load(".");
        for(auto ruleEntry : loadedRules)
        {
            auto ruleBuilder = IoC::Resolve<IRuleBuilderPtr>("JsonRuleBuilder", ruleEntry, ep);
            if(ruleBuilder)
            {
                auto rules = ruleBuilder->build();
                // std::cout << "Rules: " << rules << responce << std::endl;
                try
                {
                    rules->examine(responce);
                }
                catch (std::exception &e)
                {
                    std::cout << e.what() << std::endl;
                    continue;
                }
            }
            // std::cout << "Rule entry: " << ruleEntry << rule << std::endl;


            break;
        }
    }
    // std::cout << "Close session" << std::endl;
    // socket.shutdown(tcp::socket::shutdown_send, ec);
}

void ServerProcessCommand::execute()
{
    try {
        auto address = net::ip::address_v4::any();
        auto port = static_cast<unsigned short>(IoC::Resolve<int>("Server.Port.Get"));
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        while(IoC::Resolve<int>("Server.Shutdown.Get") == 0) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            std::thread{std::bind(&ServerProcessCommand::do_session, std::move(socket))}.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return;
    }
}
