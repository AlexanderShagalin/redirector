#ifndef RULE_PLUGIN_H
#define RULE_PLUGIN_H

#include <string>
#include "rule_plugin_global.h"
#include "irule.h"

extern "C" {
Q_DECL_EXPORT IRulePtr getRule(std::string rule, std::string field, std::string condition, IRulePtr next);
}

#endif // RULE_PLUGIN_H
