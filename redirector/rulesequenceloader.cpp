#include <filesystem>
#include <dlfcn.h>
#include <iostream>

#include "rulesequenceloader.h"

namespace json = boost::json;
namespace fs = std::filesystem;

RuleSequenceLoader::RuleSequenceLoader() : IRuleSequenceLoader() {}

std::list<json::value> RuleSequenceLoader::load(std::string path)
{
    typedef std::string* (*GetRuleFunction)(void);
    std::list<json::value> ruleList;

    try
    {
        for(const auto& entry : fs::directory_iterator(path))
        {
            if(entry.is_regular_file() && entry.path().extension() == ".so")
            {
                void *handle = dlopen(entry.path().c_str(), RTLD_LAZY);
                if(!handle)
                {
                    // std::cerr << "Error loading library: " << dlerror() << std::endl;
                    continue;
                }

                auto funtPtr = (GetRuleFunction)dlsym(handle, "getRuleSequence");
                if(!funtPtr)
                {
                    // std::cerr << "Error loading library: " << "getRuleSequence() not found" << std::endl;
                    dlclose(handle);
                    continue;
                }

                json::error_code ec;
                json::value rule = json::parse(funtPtr()->c_str(), ec);

                dlclose(handle);

                if(ec.value() != 0)
                {
                    std::cerr << "Error parse library: " << ec.message() << std::endl;
                    continue;
                }
                // std::cerr << "Library: " << rule << std::endl;
                if(rule.is_array())
                {
                    for(const auto &r : rule.as_array())
                    {
                        ruleList.push_back(r);
                    }
                }
                else if(rule.is_object())
                {
                    ruleList.push_back(rule);
                }
            }
        }
    }
    catch(const fs::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return ruleList;
}
