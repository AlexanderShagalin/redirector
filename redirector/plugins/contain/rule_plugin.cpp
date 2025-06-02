#include "rule_plugin.h"

#include "rulecontain.h"

IRulePtr getRule(std::string rule, std::string field, std::string condition, IRulePtr next)
{
    if(rule == "Contain")
        return new RuleContain(field, condition, next);
    return nullptr;
}

IRulePtr allocateRule(std::string rule, std::string field, std::string condition, IRulePtr next, void* buffer)
{
    if(rule == "Contain")
    {
        auto rawPtr = new (buffer) RuleContain(field, condition, next);
        return rawPtr;
    }
    return nullptr;
}
