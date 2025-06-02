#include <boost/json.hpp>
#include "ioc.h"
#include "sendnorulecommand.h"


namespace json = boost::json;

SendNoRuleCommand::SendNoRuleCommand(sockaddr_in client) : ICommand(),
    m_client(client)
{}

void SendNoRuleCommand::execute()
{
    json::object errorValue;
    errorValue["error"] = "no_rule";
    std::string str = json::serialize(errorValue);
    if(!IoC::Resolve<bool>("Connector.Write", (const char*)str.c_str(), (int)str.size(), m_client))
    {
        throw std::runtime_error("SendErrorCommand: connector's error");
        return;
    }
}
