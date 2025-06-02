#include "rule_plugin.h"

#include "rulegtdate.h"

IRulePtr getRule(std::string rule, std::string field, std::string condition, IRulePtr next)
{
    if(rule == "GTDate")
        return new RuleGTDate(field, condition, next);
    return nullptr;
}

IRulePtr allocateRule(std::string rule, std::string field, std::string condition, IRulePtr next, void* buffer)
{
    if(rule == "GTDate")
    {
        auto rawPtr = new (buffer) RuleGTDate(field, condition, next);
        return rawPtr;
    }
    return nullptr;
}
