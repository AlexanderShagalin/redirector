#include <filesystem>
#include <dlfcn.h>
#include <iostream>
#include <link.h>

#include "ruleloader.h"

namespace json = boost::json;
namespace fs = std::filesystem;

IRuleLoaderPtr RuleLoader::m_instance = nullptr;

RuleLoader::RuleLoader(std::string path) :
    IRuleLoader(),
    m_path(path)
{

}

// IRuleLoaderPtr RuleLoader::getInstance()
// {
//     if(m_instance == nullptr)
//         m_instance = std::shared_ptr<RuleLoader>(new RuleLoader(std::string(".")));
//     return m_instance;
// }

IRulePtr RuleLoader::load(std::string rule, std::string field, std::string condition, IRulePtr next)
{
    typedef IRulePtr (*GetRuleFunction)(std::string, std::string, std::string, IRulePtr);
    // typedef IRulePtr (*AllocateRuleFunction)(std::string, std::string, std::string, IRulePtr, void*);

    try
    {
        for(const auto& entry : fs::directory_iterator(m_path))
        {
            if(entry.is_regular_file() && entry.path().extension() == ".so")
            {
                void *handle = dlopen(entry.path().c_str(), RTLD_LAZY);
                if(!handle)
                {
                    // std::cerr << "Error loading library: " << dlerror() << std::endl;
                    continue;
                }

                auto funtPtr = (GetRuleFunction)dlsym(handle, "getRule");
                // auto funtPtr = (AllocateRuleFunction)dlsym(handle, "allocateRule");
                if(!funtPtr)
                {
                    // std::cerr << "Error loading library: " << "getRule() not found" << std::endl;
                    dlclose(handle);
                    continue;
                }


                auto result = funtPtr(rule, field, condition, next);
                if(!result)
                {
                    dlclose(handle);
                    continue;
                }

                // dlclose(handle);

                std::cout << "RuleLoader result: " << entry.path().c_str() << " " << result << std::endl;
                return result;
            }
        }
    }
    catch(const fs::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return nullptr;
}
