#ifndef IRULEBUILDER_H
#define IRULEBUILDER_H

#include "irule.h"

class IRuleBuilder
{
public:
    IRuleBuilder() = default;
    ~IRuleBuilder() = default;

    virtual IRulePtr build() = 0;
};

typedef std::shared_ptr<IRuleBuilder> IRuleBuilderPtr;

#endif // IRULEBUILDER_H
