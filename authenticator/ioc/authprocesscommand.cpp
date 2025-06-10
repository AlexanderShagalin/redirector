#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>
#include <boost/program_options.hpp>
#include <jwt-cpp/jwt.h>


#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "authprocesscommand.h"
#include "ioc.h"

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

// СуперСекретный ключ для подписи JWT
const std::string SECRET_KEY = "my_super_secret_key!";

using namespace std;
// namespace json = boost::json;
namespace opt = boost::program_options;
namespace fs = std::filesystem;


// Генерация JWT токена
std::string generate_token(const std::string& username) {
    auto token = jwt::create()
                     .set_issuer("auth0")
                     .set_type("JWS")
                     .set_payload_claim("username", jwt::claim(username))
                     .set_issued_at(std::chrono::system_clock::now())
                     .set_expires_at(std::chrono::system_clock::now() + std::chrono::minutes{1})
                     .sign(jwt::algorithm::hs256{SECRET_KEY});

    return token;
}

// Верификация JWT токена
bool verify_token(const std::string& token, std::string& username) {
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
                            .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
                            .with_issuer("auth0");

        verifier.verify(decoded);

        username = decoded.get_payload_claim("username").as_string();
        return true;
    } catch (...) {
        return false;
    }
}

AuthProcessCommand::AuthProcessCommand() : ICommand() {}

void AuthProcessCommand::do_session(tcp::socket& socket)
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

boost::beast::http::response<http::string_body> AuthProcessCommand::handle_request(const boost::beast::http::request<boost::beast::http::string_body> &req)
{
    http::response<http::string_body> res;

    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");

    if(req.target() == "/register" && req.method() == http::verb::post)
    {
        json::error_code ec;
        std::string body = req.body();
        auto userData = json::parse(body, ec);
        // std::cout << ec.value() << userData;
        if(ec.value() == 0 && userData.as_object().contains("username"))
        {
            auto token = generate_token(std::string(userData.as_object()["username"].as_string()));
            res.set(http::field::authentication_results, token);
            std::string str;
            // str += "<html><head><title>Authentificator</title><meta charset=\"utf8\"/></head>"
            //        "<body><h1>";

            str += token;

            // str += "</h1>"
            //        "</body></html>";
            std::cout << str << std::endl;
            res.body() = str;
        }
        else
        {
            res.result(http::status::unauthorized);
            res.set(http::field::authentication_info, "sdfs");
            res.body() = "<html><head><title>Authentificator</title><meta charset=\"utf8\"/></head>"
                         "<body><h1>Не авторизован!</h1>"
                         "</body></html>";

        }
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
        json::error_code ec;
        std::string auth_data = req[http::field::authentication_info];
        std::cout << "auth_data: " << auth_data << std::endl;

        auto userData = json::parse(auth_data, ec);
        if(ec.value() == 0
            && userData.as_object().contains("username")
            && userData.as_object().contains("token"))
        {
            std::string decodedName;
            if(verify_token(std::string(userData.as_object()["token"].as_string()),
                            decodedName))
            {
                res.set(http::field::authentication_info, decodedName);
                res.prepare_payload();
                res.keep_alive(req.keep_alive());
                std::cout << "Client authorized" << std::endl;
                return res;
            }
        }
        res.result(http::status::unauthorized);
        res.body() = "<html><head><title>Authentificator</title><meta charset=\"utf8\"/></head>"
                     "<body><h1>Ошибка авторизации!</h1>"
                     "</body></html>";
    }

    res.prepare_payload();
    res.keep_alive(req.keep_alive());
    return res;
}

void AuthProcessCommand::execute()
{
    try {
        auto const address = net::ip::address_v4::any();
        auto const port = static_cast<unsigned short>(IoC::Resolve<int>("Server.Port.Get"));
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        while(IoC::Resolve<int>("Server.Shutdown.Get") == 0) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            std::thread{std::bind(&AuthProcessCommand::do_session, std::move(socket))}.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return;
    }
}
