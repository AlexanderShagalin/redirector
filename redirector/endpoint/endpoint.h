#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <boost/asio/ip/tcp.hpp>

#include "iendpoint.h"

class EndPoint : public IEndPoint
{
    boost::asio::ip::tcp::socket &m_socket;
public:
    EndPoint(boost::asio::ip::tcp::socket &socket);

    // IEPoint interface
public:
    std::string read();
    void write(std::string data);
    void writeDone(std::string);
    void writeError(std::string);
};

#endif // ENDPOINT_H
