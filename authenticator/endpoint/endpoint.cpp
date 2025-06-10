#include <stdexcept>
#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include "endpoint.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

EndPoint::EndPoint(boost::asio::ip::tcp::socket& socket) : IEndPoint(),
    m_socket(socket)
{}

std::string EndPoint::read()
{
    beast::error_code ec;
    beast::flat_buffer buffer;

    http::request<http::string_body> req;
    try
    {
        http::read(m_socket, buffer, req, ec);
    }
    catch(std::exception &e)
    {
        throw e;
    }

    if(ec == http::error::end_of_stream)
    {
        m_socket.shutdown(tcp::socket::shutdown_send, ec);
        throw std::runtime_error("EOF recieved");
    }

    if(ec) {
        throw std::runtime_error(ec.message());
    }

    std::string data = req.body();
    return data;
}

void EndPoint::write(std::string data)
{
    http::response<http::string_body> res;
    beast::error_code ec;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = data;
    res.prepare_payload();
    res.keep_alive(true);

    try
    {
        http::write(m_socket, res, ec);
    }
    catch(std::exception &e)
    {
        throw e;
    }


    if(ec) {
        throw std::runtime_error(ec.message());
    }
}

void EndPoint::writeDone(std::string data)
{
    http::response<http::string_body> res;
    beast::error_code ec;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = data;
    res.prepare_payload();
    res.keep_alive(true);

    try
    {
        std::cout << "EndPoint: " << res << std::endl;
        http::write(m_socket, res, ec);
    }
    catch(std::exception &e)
    {
        throw e;
    }

    if(ec) {
        throw std::runtime_error(ec.message());
    }
}

void EndPoint::writeError(std::string data)
{
    http::response<http::string_body> res;
    beast::error_code ec;

    res.result(http::status::not_acceptable);
    res.set(http::field::content_type, "application/json");
    res.body() = data;
    res.prepare_payload();
    res.keep_alive(true);

    try
    {
        http::write(m_socket, res, ec);
    }
    catch(std::exception &e)
    {
        throw e;
    }

    if(ec) {
        throw std::runtime_error(ec.message());
    }
}
