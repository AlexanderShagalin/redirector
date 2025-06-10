#ifndef RULEEQUAL_H
#define RULEEQUAL_H

#include <boost/json.hpp>
#include <irule.h>

class RuleEqual : public IRule
{
    std::string m_rule;
    std::string m_condition;
    boost::json::value m_jvalue;
    IRulePtr m_next = nullptr;

public:
    RuleEqual(std::string rule, std::string condition, IRulePtr next = nullptr);

    // IRule interface
public:
    void examine(const boost::json::value &jrequest);
};

#endif // RULEEQUAL_H
