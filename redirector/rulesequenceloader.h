#ifndef RULESEQUENCELOADER_H
#define RULESEQUENCELOADER_H

#include "irulesequenceloader.h"

class RuleSequenceLoader : public IRuleSequenceLoader
{
public:
    RuleSequenceLoader();

    // IRuleLoader interface
public:
    std::list<boost::json::value> load(std::string path);
};

#endif // RULESEQUENCELOADER_H
