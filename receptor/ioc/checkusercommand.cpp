#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

#include "checkusercommand.h"

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

// Выполняет HTTP GET запрос и выводит ответ
void do_request(const std::string& host, const std::string& port, const std::string& token, const std::string& username)
{
    // Контекст ввода-вывода, необходимый для всех операций ввода-вывода
    net::io_context ioc;

    // Эти объекты выполняют наш ввод-вывод
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Поиск доменного имени
    auto const results = resolver.resolve(host, port);

    // Установка соединения по IP-адресу, который мы получили из поиска
    stream.connect(results);

    // Установка HTTP POST запроса
    http::request<http::string_body> req{http::verb::post, "/", 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    // req.set(http::field::accept, "text/html"); // Пример пользовательского заголовка
    // req.set("X-Custom-Header", "MyValue");    // Пример кастомного заголовка

    std::string body = "{\"username\":\"";
    body += username;
    body += "\",\"token\":\"";
    body += token;
    body += "\"}";


    req.set(http::field::authentication_info, body);
    req.prepare_payload();


    // Отправка HTTP запроса на удаленный хост
    http::write(stream, req);

    // Этот буфер используется для чтения и должен быть сохранен
    beast::flat_buffer buffer;

    // Объявление контейнера для хранения ответа
    http::response<http::dynamic_body> res;

    // Получение HTTP ответа
    http::read(stream, buffer, res);

    // Вывод ответа
    std::cout << res[http::field::authentication_results] << std::endl;

    // Корректное закрытие сокета
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    std::cout << res.result() << std::endl;
    if(res.result() != http::status::ok)
        throw std::runtime_error("CheckUserCommand: unauthenticated");
}

CheckUserCommand::CheckUserCommand(std::string token, std::string username) :
    ICommand(),
    m_token(token),
    m_username(username)
{}

void CheckUserCommand::execute()
{
    do_request("auth", "28082", m_token, m_username);
}
