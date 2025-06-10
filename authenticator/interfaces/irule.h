#ifndef IRULE_H
#define IRULE_H

#include <boost/json.hpp>
#include <memory>

class IRule
{
public:
    IRule() = default;
    ~IRule() = default;

    virtual void examine(const boost::json::value &jrequest) = 0;
};

// typedef std::shared_ptr<IRule> IRulePtr;
typedef IRule* IRulePtr;

#endif // IRULE_H
