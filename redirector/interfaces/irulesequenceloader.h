#ifndef IRULESEQUENCELOADER_H
#define IRULESEQUENCELOADER_H

#include <boost/json.hpp>
#include <string>
#include <list>

class IRuleSequenceLoader
{
public:
    IRuleSequenceLoader() = default;
    ~IRuleSequenceLoader() = default;

    virtual std::list<boost::json::value> load(std::string path = ".") = 0;
};

typedef std::shared_ptr<IRuleSequenceLoader> IRuleSequenceLoaderPtr;

#endif // IRULESEQUENCELOADER_H
