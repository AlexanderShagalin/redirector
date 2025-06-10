#ifndef AUTHPROCESSCOMMAND_H
#define AUTHPROCESSCOMMAND_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>

#include <icommand.h>

class AuthProcessCommand : public ICommand
{
public:
    AuthProcessCommand();

    static void do_session(boost::asio::ip::tcp::socket& socket);
    static boost::beast::http::response<boost::beast::http::string_body> handle_request(const boost::beast::http::request<boost::beast::http::string_body>& req);

    // ICommand interface
public:
    void execute();
};

#endif // AUTHPROCESSCOMMAND_H
