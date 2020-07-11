#include "../common/cslbase.h"

uintptr_t PropertyTrigger::AddNotification(PropertyNotification&& pn)
{
    this->m_vecPN.push_back(pn);
    return m_vecPN.size();
}
void PropertyTrigger:: Fire(uint32_t uID)
{
    for(auto i:m_vecPN)
    {
        i(uID);
    }
};


