#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "serverprocesscommand.h"
#include "ioc.h"
#include "iredirector.h"

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

ServerProcessCommand::ServerProcessCommand() : ICommand() {}

void ServerProcessCommand::do_session(tcp::socket& socket)
{
    beast::error_code ec;
    beast::flat_buffer buffer;

    std::cout << "Create session" << std::endl;

    for(;;)
    {
        http::request<http::string_body> req;
        http::read(socket, buffer, req, ec);
        if(ec == http::error::end_of_stream)
        {
            break;
        }

        if(ec) {
            std::cerr << "Ошибка чтения: " << ec.message() << "\n";
            socket.shutdown(tcp::socket::shutdown_send, ec);
            return;
        }

        http::response<http::string_body> res = handle_request(req);
        http::write(socket, res, ec);
        if(ec) {
            std::cerr << "Ошибка записи: " << ec.message() << "\n";
            socket.shutdown(tcp::socket::shutdown_send, ec);
            return;
        }
    }
    std::cout << "Close session" << std::endl;
    socket.shutdown(tcp::socket::shutdown_send, ec);
}

boost::beast::http::response<http::string_body> ServerProcessCommand::handle_request(const boost::beast::http::request<boost::beast::http::string_body> &req)
{
    http::response<http::string_body> res;

    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");

    if(req.target() == "/")
    {
        res.body() = "<html><head><title>Receptor</title><meta charset=\"utf8\"/></head>"
                     "<body><h1>Добро пожаловать!</h1>"
                     "</body></html>";
    }
    else if(req.target() == "/shutdown")
    {
        IoC::Resolve<void>("Server.Shutdown.Set", 1);
        res.body() = "<html><head><title>Receptor</title><meta charset=\"utf8\"/></head>"
                     "<body><h1>Приложение остановлено!</h1>"
                     "</body></html>";
    }
    else
    {
        json::object obj;
        obj["target"] = req.target();
        for(const auto &field : req)
        {
            obj[field.name_string()] = req[field.name_string()];
        }

        std::string str = json::serialize(obj);
        // std::cout << str << std::endl;
        auto redirector = IoC::Resolve<IRedirectorPtr>("Redirector.Get");
        std::string result;
        if(redirector)
            result = redirector->request(str);

        std::cout << "Redirector.Get: " << result << std::endl;

        if(result.empty())
        {
            res.result(http::status::internal_server_error);
            res.body() = "<html><head><title>Ошибка сервера</title><meta charset=\"utf8\"/></head>"
                         "<body><h1>500 Internal server error</h1>"
                         "</body></html>";
        }
        else
        {
            json::error_code ec;
            auto recieved = json::parse(result, ec);
            // std::cout << recieved << std::endl;
            if(ec.value() == 0)
            {
                if(recieved.as_object().if_contains("error"))
                {
                    res.body() = "<html><head><title>Ошибка</title><meta charset=\"utf8\"/></head>"
                                 "<body><h1>Правило не определено</h1>"
                                 "</body></html>";
                }
                else if(recieved.as_object().if_contains("redirect"))
                {
                    res.result(http::status::temporary_redirect );
                    res.set(http::field::location, recieved.as_object()["redirect"].as_string());
                    res.body() = "<html><head><title>Перемещено</title><meta charset=\"utf8\"/></head>"
                                 "<body><h1>307 Temporary redirect</h1>"
                                 "</body></html>";
                }
            }
        }
    }

    res.prepare_payload();
    res.keep_alive(req.keep_alive());
    return res;
}

void ServerProcessCommand::execute()
{
    try {
        auto const address = net::ip::address_v4::any();
        auto const port = static_cast<unsigned short>(8080);
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        while(IoC::Resolve<int>("Server.Shutdown.Get") == 0) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            std::thread{std::bind(&ServerProcessCommand::do_session, std::move(socket))}.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return;
    }
}
