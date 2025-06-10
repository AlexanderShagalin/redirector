#include "rule_plugin.h"

#include "ruleequal.h"

IRulePtr getRule(std::string rule, std::string field, std::string condition, IRulePtr next)
{
    if(rule == "Equal")
        return new RuleEqual(field, condition, next);
    return nullptr;
}
