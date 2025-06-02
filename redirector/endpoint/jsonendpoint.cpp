#include <boost/json.hpp>
#include "jsonendpoint.h"

namespace json = boost::json;

JsonEndPoint::JsonEndPoint(IEndPointPtr ep) : IEndPoint(), m_ep(ep){}

std::string JsonEndPoint::read()
{
    json::error_code ec;
    auto buffer = m_ep->read();

    json::parse(buffer, ec);
    if( ec.value() != 0)
    {
        return std::string();
    }

    return buffer;
}

void JsonEndPoint::writeDone(std::string data)
{
    json::object doneValue;
    doneValue["redirect"] = data;
    m_ep->writeDone(json::serialize(doneValue));
}

void JsonEndPoint::writeError(std::string data)
{
    json::object errorValue;
    errorValue["error"] = data;
    m_ep->writeError(json::serialize(errorValue));
}
