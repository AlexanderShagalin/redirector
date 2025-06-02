#ifndef IREDIRECTOR_H
#define IREDIRECTOR_H

#include <string>
#include <memory>

class IRedirector
{
public:
    IRedirector() = default;
    ~IRedirector() = default;

    virtual std::string request(std::string str) = 0;
};

typedef std::shared_ptr<IRedirector> IRedirectorPtr;

#endif // IREDIRECTOR_H
