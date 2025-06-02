#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <iostream>

#include "httpredirector.h"

using namespace std;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

HttpRedirector::HttpRedirector() : IRedirector() {}

std::string HttpRedirector::request(std::string str)
{
    try
    {
        net::io_context ioc{1};
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream{ioc};

        stream.connect(resolver.resolve("127.0.0.1", "18081"));

        http::request<http::string_body> req{http::verb::post, "/", 10};
        req.set(http::field::host, "127.0.0.1");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = str;
        req.prepare_payload();
        http::write(stream, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        // std::cout << res << std::endl;
        std::string resBody = res.body();
        stream.socket().shutdown(tcp::socket::shutdown_both);
        return resBody;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return std::string();
}
