#include "rule_plugin.h"

std::string *getRuleSequence()
{
    // return new std::string(
    //     R"(
    //     [
    //     {
    //        "target": "https://rambler.ru",
    //        "rules": [
    //                     {"type": "GTDate", "rule": "Date", "condition": "18.05.2025"}
    //                 ]
    //     }
    //     ]
    //     )"
    //     );

    return new std::string(
        R"(
        [
        {
           "target": "https://mail.ru",
           "rules": [
                        {"type": "Contain", "rule": "User-Agent", "condition": "Firefox"},
                        {"type": "Contain", "rule": "Accept-Language", "condition": "ru-RU"}
                    ]
        },
        {
           "target": "https://rambler.ru",
           "rules": [
                        {"type": "GTDate", "rule": "Date", "condition": "18.05.2025"}
                    ]
        },
        {
           "target": "https://ya.ru",
           "rules": [
                        {"type": "Contain", "rule": "User-Agent", "condition": "Chrome"},
                        {"type": "Contain", "rule": "Accept-Language", "condition": "ru-RU"}
                    ]
        }
        ]
)"
        );

}
