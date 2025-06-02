#ifndef IRULELOADER_H
#define IRULELOADER_H

#include <string>
#include "irule.h"

class IRuleLoader
{
public:
    IRuleLoader() = default;
    ~IRuleLoader() = default;

    virtual IRulePtr load(std::string rule, std::string field, std::string condition, IRulePtr next) = 0;
};

typedef std::shared_ptr<IRuleLoader> IRuleLoaderPtr;

#endif // IRULELOADER_H
