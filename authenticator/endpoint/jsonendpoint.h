#ifndef JSONENDPOINT_H
#define JSONENDPOINT_H

#include "iendpoint.h"

class JsonEndPoint : public IEndPoint
{
    IEndPointPtr m_ep;
public:
    JsonEndPoint(IEndPointPtr ep);

    // IEPoint interface
public:
    std::string read();
    void writeDone(std::string data);
    void writeError(std::string data);
};

#endif // JSONENDPOINT_H
