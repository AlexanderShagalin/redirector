#ifndef JSONRULEBUILDER_H
#define JSONRULEBUILDER_H

#include <boost/json.hpp>
#include "irulebuilder.h"
#include "iendpoint.h"
#include "irule.h"

class JsonRuleBuilder : public IRuleBuilder
{
    boost::json::value m_jrules;
    IEndPointPtr m_ep;
public:
    JsonRuleBuilder(const boost::json::value &jrules, IEndPointPtr ep);

    // IRuleBuilder interface
public:
    IRulePtr build();
};

#endif // JSONRULEBUILDER_H
