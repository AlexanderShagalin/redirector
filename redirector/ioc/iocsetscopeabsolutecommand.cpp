#include "iocsetscopeabsolutecommand.h"
#include <stdexcept>
#include <sstream>
#include <string>


using namespace std;

IocSetScopeAbsoluteCommand::IocSetScopeAbsoluteCommand(IScopePtr root, IScopeChangerPtr changer, string targetScope)
    : m_pRoot(root), m_pChanger(changer), m_sTargetScope(targetScope)
{
}

void IocSetScopeAbsoluteCommand::execute()
{
    istringstream stream(m_sTargetScope);
    string token;
    const char delimiter = '/';

    if (!getline(stream, token, delimiter))
        throw std::runtime_error("IocSetScopeAbsoluteCommand::execute(): wrong path");

    if (token != "")
        throw std::runtime_error("IocSetScopeAbsoluteCommand::execute(): wrong path");

    IScopePtr scope = m_pRoot;

    while (getline(stream, token, delimiter)) {
        if (token == "")
            throw std::runtime_error("IocSetScopeAbsoluteCommand::execute(): wrong path");
        IScopePtr next = scope->FindChild(token);
        if (next == nullptr)
            throw std::runtime_error("IocSetScopeAbsoluteCommand::execute(): wrong path");
        scope = next;
    }

    m_pChanger->Change(scope);
}
