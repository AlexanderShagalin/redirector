#include <stdexcept>
#include "ruleequal.h"

RuleEqual::RuleEqual(std::string rule, std::string condition, IRulePtr next) : IRule(),
    m_rule(rule),
    m_condition(condition),
    m_next(next)
{}

void RuleEqual::examine(const boost::json::value &jrequest)
{
    if(!jrequest.is_object())
    {
        throw std::runtime_error("RuleEqual: wrong rule");
        return;
    }

    if(!jrequest.as_object().if_contains(m_rule))
    {
        throw std::runtime_error("RuleEqual: rule not exist");
        return;
    }

    if(jrequest.at(m_rule).as_string() != m_condition)
    {
        std::string exc("RuleEqual: condition is not equal");
        throw std::runtime_error(exc);
        return;
    }

    if(m_next)
        m_next->examine(jrequest);
}
