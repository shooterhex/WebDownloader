#include "../common/cslbase.h"
/*
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
*/
PropertyTrigger::PropertyTrigger() noexcept
{
}
PropertyTrigger::~PropertyTrigger() noexcept
{
}

void PropertyTrigger::ClearNotifications() noexcept
{
    m_vecPN.clear();
}

uintptr_t PropertyTrigger::AddNotification(PropertyNotification&& pn)
{
    uintptr_t index = 0;
    for ( auto& fn : m_vecPN ) {
        if ( fn == nullptr ) {
            fn = std::move(pn);
            return index + 1;
        }
        index ++;
    }
    m_vecPN.push_back(std::move(pn));
    return index + 1;
}

void PropertyTrigger::RemoveNotification(uintptr_t uCookie) noexcept
{
    assert( uCookie > 0 && uCookie <= m_vecPN.size() );
    m_vecPN[uCookie - 1] = nullptr;
}

void PropertyTrigger::Fire(uint32_t uID)
{
    for ( auto& fn : m_vecPN ) {
        if ( fn != nullptr )
            fn(uID);
    }
}
