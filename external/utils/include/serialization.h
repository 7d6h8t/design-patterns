#pragma once
#include <cstdint>
#include <utility>
#include <exception>
#include <type_traits>

namespace utils {
    class serialization final
    {
    private:
        serialization() = delete;
        virtual ~serialization() = delete;
        //none copyable
        serialization(const serialization&) = delete;
        serialization& operator=(const serialization&) = delete;
        //none movable
        serialization(serialization&&) = delete;
        serialization& operator=(serialization&&) = delete;

    public:
        template<typename Byte_t,
            typename _Ty,
            std::enable_if_t<std::is_class<_Ty>::value == false, bool> = true>
            static Byte_t serialize(const _Ty& data)
        {
            size_t nSize = sizeof(_Ty);
            Byte_t byte(nSize, 0);

            size_t i = 0;
            for (auto itr = byte.rbegin(); itr != byte.rend(); ++itr, ++i)
            {
                *itr = static_cast<typename Byte_t::value_type>(data >> i * 8) | static_cast<typename Byte_t::value_type>(0);
            }

            return std::move(byte);
        }

        template<typename _Ty,
            typename Byte_t,
            std::enable_if_t<std::is_class<_Ty>::value == false, bool> = true>
            static _Ty deserialize(const Byte_t& byte)
        {
            size_t nSize = sizeof(_Ty);
            if (nSize < byte.size())
                throw std::logic_error("Byte size exceeded");

            _Ty data{};

            for (auto itr = byte.cbegin(); itr != byte.cend(); ++itr)
            {
                data = data << 8 | static_cast<_Ty>(*itr);
            }

            return std::move(data);
        }

    }; //class serialization
} //namespace utils
