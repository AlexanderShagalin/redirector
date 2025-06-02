#include <iostream>
#include <boost/json.hpp>

#include "ioc.h"
#include "jsonrulebuilder.h"
#include "iruleloader.h"

namespace json = boost::json;

JsonRuleBuilder::JsonRuleBuilder(const json::value &jrules, IEndPointPtr ep) :
    IRuleBuilder(),
    m_jrules(jrules),
    m_ep(ep)
{}

IRulePtr JsonRuleBuilder::build()
{
    if(m_jrules.at("target").is_null())
        return nullptr;

    if(m_jrules.at("rules").is_null())
        return nullptr;

    std::string target = json::value_to<std::string>(m_jrules.at("target"));

    IRulePtr rule = IoC::Resolve<IRulePtr>("Rules.Redirect", target , m_ep);
    for(auto item : m_jrules.at("rules").as_array())
    {
        if(!item.as_object().if_contains("type"))
            continue;
        if(!item.as_object().if_contains("rule"))
            continue;
        if(!item.as_object().if_contains("condition"))
            continue;

        std::cout << "Build rule:"
                  << item.as_object()["type"]
                  << item.as_object()["condition"]
                  << item.as_object()["rule"] << std::endl;

        try
        {
            auto loader = IoC::Resolve<IRuleLoaderPtr>("Rules.Plugins.Loader");
            auto loaded = loader->load(item.as_object()["type"].as_string().c_str(), std::string(item.as_object()["rule"].as_string()), std::string(item.as_object()["condition"].as_string()), rule);
            std::cout << "Load for "
                      << "rule: " << item.as_object()["type"].as_string().c_str() << " "
                      << "loaded: " << loaded << " "
                      << std::endl;

            if(!loaded)
                throw std::runtime_error(std::string("JsonRuleBuilder: rule not found ") + std::string(item.as_object()["type"].as_string()));
            rule = loaded;
         }
        catch(std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    return rule;
}
