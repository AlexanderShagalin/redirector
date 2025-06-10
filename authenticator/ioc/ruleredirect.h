#ifndef RULEREDIRECT_H
#define RULEREDIRECT_H

#include <string>
#include <netinet/in.h>

#include <irule.h>
#include "iendpoint.h"


class RuleRedirect : public IRule
{
    IEndPointPtr m_ep;
    std::string m_target;
public:
    RuleRedirect(std::string target, IEndPointPtr ep);


    // IRule interface
public:
    void examine(const boost::json::value &jrequest);
};

#endif // RULEREDIRECT_H
