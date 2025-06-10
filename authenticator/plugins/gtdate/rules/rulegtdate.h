#ifndef RULEGTDATE_H
#define RULEGTDATE_H

#include <irule.h>

class RuleGTDate : public IRule
{
    std::string m_rule;
    std::string m_condition;
    IRulePtr m_next = nullptr;

public:
    RuleGTDate(std::string rule, std::string condition, IRulePtr next = nullptr);



    // IRule interface
public:
    void examine(const boost::json::value &jrequest);
};

#endif // RULEGTDATE_H
