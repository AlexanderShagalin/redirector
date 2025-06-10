#ifndef RULELOADER_H
#define RULELOADER_H

#include "map"

#include "iruleloader.h"


class RuleLoader : public IRuleLoader
{
    std::string m_path;
    std::map<std::string, IRulePtr> m_rules;
    static IRuleLoaderPtr m_instance;


public:
    RuleLoader(std::string path);
    // static IRuleLoaderPtr getInstance();

    // IRuleLoader interface
public:
    IRulePtr load(std::string rule, std::string field, std::string condition, IRulePtr next);
};

#endif // RULELOADER_H
