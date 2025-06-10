#include <iostream>
#include <stdexcept>
#include "rulecontain.h"

RuleContain::RuleContain(std::string rule, std::string condition, IRulePtr next) : IRule(),
    m_rule(rule),
    m_condition(condition),
    m_next(next)
{}

void RuleContain::examine(const boost::json::value &jrequest)
{
    std::cout << "RuleContain examine" <<std::endl;
    if(!jrequest.is_object())
    {
        throw std::runtime_error("RuleContainCommand: wrong rule");
        return;
    }

    // std::cout << m_jrequest << " " << m_rule << std::endl;

    if(!jrequest.as_object().if_contains(m_rule))
    {
        throw std::runtime_error("RuleContainCommand: rule not exist");
        return;
    }

    if(jrequest.at(m_rule).as_string().find(m_condition) == std::string::npos)
    {
        std::string exc("RuleContainCommand: condition is not contain");
        exc += std::string(" ");
        exc += jrequest.at(m_rule).as_string();
        exc += std::string(" ");
        throw std::runtime_error(exc);
        return;
    }

    if(m_next)
        m_next->examine(jrequest);
}
