#ifndef IENDPOINT_H
#define IENDPOINT_H

#include "string"
#include "memory"

class IEndPoint
{
public:
    IEndPoint() = default;
    ~IEndPoint() = default;
    virtual std::string read() = 0;

    virtual void writeDone(std::string) = 0;
    virtual void writeError(std::string) = 0;
};

typedef std::shared_ptr<IEndPoint> IEndPointPtr;
#endif // IENDPOINT_H
