#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "rulegtdate.h"

RuleGTDate::RuleGTDate(std::string rule, std::string condition, IRulePtr next) :
    IRule(),
    m_rule(rule),
    m_condition(condition),
    m_next(next)
{

}

void RuleGTDate::examine(const boost::json::value &jrequest)
{
    std::cout << "RuleGTDate examine" <<std::endl;
    std::istringstream ss(m_condition);
    std::tm tm;
    ss >> std::get_time(&tm, "%d.%m.%Y");

    auto now = std::time(nullptr);
    auto current = std::localtime(&now);
    now = std::mktime(current);

    std::cout << "Examine: " << tm.tm_year << current->tm_year << m_condition <<std::endl;

    if(ss.fail())
        throw std::runtime_error("RuleGTDate: parse date exception");

    if(current->tm_year >= tm.tm_year)
    {
        if(current->tm_yday >= tm.tm_yday)
        {
            if(m_next)
                m_next->examine(jrequest);
            return;
        }
    }
    throw std::runtime_error("RuleGTDate: date is not greather exception");
}
