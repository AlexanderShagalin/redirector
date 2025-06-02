#ifndef HTTPREDIRECTOR_H
#define HTTPREDIRECTOR_H

#include "iredirector.h"

class HttpRedirector : public IRedirector
{
public:
    HttpRedirector();

    // IRedirector interface
public:
    std::string request(std::string str);
};

#endif // HTTPREDIRECTOR_H
