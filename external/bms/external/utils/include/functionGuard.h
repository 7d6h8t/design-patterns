#pragma once
#include <functional>

namespace utils {
    class functionalGuard final
    {
    public:
        typedef std::function<void(void)> funcType;
    public:
        functionalGuard() = delete;
        functionalGuard(const functionalGuard&) = delete;
        functionalGuard(functionalGuard&& other) = delete;

        functionalGuard& operator=(const functionalGuard& other) = delete;
        functionalGuard& operator=(functionalGuard&& other) = delete;

        functionalGuard(funcType&& fLocker, funcType&& fUnlocker) noexcept
        {
            try
            {
                m_fLocker = std::move(fLocker);
                m_fUnLocker = std::move(fUnlocker);

                m_fLocker();
            } catch (const std::exception&)
            {
            }

        }
        ~functionalGuard()
        {
            try
            {
                m_fUnLocker();
            } catch (const std::exception&)
            {
            }
        }
    protected:
        funcType m_fLocker;
        funcType m_fUnLocker;
    };//class functionalGuard
}//namespace utils
