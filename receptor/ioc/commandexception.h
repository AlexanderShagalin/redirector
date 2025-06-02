#ifndef COMMANDEXCEPTION_H
#define COMMANDEXCEPTION_H

#include <string>
#include <exception>
#include <memory>

class CommandException : public std::exception
{
    std::string m_what;
public:
    CommandException(std::string w = "") { m_what = w; }
    CommandException(const std::exception &e) { m_what = std::string(e.what()); }
    const char*
    what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return m_what.data(); }
};

#endif // RETRYEXCEPTION_H
