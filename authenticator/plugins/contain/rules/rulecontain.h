#ifndef RULECONTAIN_H
#define RULECONTAIN_H

#include <boost/json.hpp>
#include <irule.h>



class RuleContain : public IRule
{
    std::string m_rule;
    std::string m_condition;
    IRulePtr m_next = nullptr;


public:
    RuleContain(std::string rule, std::string condition, IRulePtr next = nullptr);

    // IRule interface
public:
    void examine(const boost::json::value &jrequest);
};

#endif // RULECONTAIN_H
