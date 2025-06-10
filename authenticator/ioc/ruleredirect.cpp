#include <iostream>

#include "ruleredirect.h"

RuleRedirect::RuleRedirect(std::string target, IEndPointPtr ep) : IRule(),
    m_ep(ep), m_target(target)
{}

void RuleRedirect::examine(const boost::json::value &jrequest)
{
    if(!m_ep)
        throw std::runtime_error("RedirectRule: ep is null");

    try
    {
        m_ep->writeDone(m_target);
        std::cout << "RuleRedirect: redirected to " << m_target << std::endl;
    }
    catch(std::exception &e)
    {
        throw std::runtime_error("RedirectRule: send error");
    }
}

