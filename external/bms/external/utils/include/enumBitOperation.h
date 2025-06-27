#pragma once

#include <type_traits>

//
// MS STL <regex> 헤더의 _REGEX_BITMASK_OPS 매크로
// Scoped Enum의 비트 연산을 위해 사용됨
//

/*
    사용 예시)

    enum class EnumType : uint32_t
    {
        AAA = 1 << 0,
        BBB = 1 << 1,
        CCC = 1 << 2
    };
    DEFINE_BITMASK_OPERATION(EnumType);

    EnumType a = EnumType::AAA | EnumType::BBB;
*/

#define DEFINE_BITMASK_OPERATION(Ty) \
inline Ty& operator&=(Ty& _Left, Ty _Right) \
{   /* return _Left &= _Right */ \
    _Left = (Ty)((std::underlying_type<Ty>::type)_Left & (std::underlying_type<Ty>::type)_Right); return (_Left); \
} \
\
inline Ty& operator|=(Ty& _Left, Ty _Right) \
{   /* return _Left |= _Right */ \
    _Left = (Ty)((std::underlying_type<Ty>::type)_Left | (std::underlying_type<Ty>::type)_Right); return (_Left); \
} \
\
inline Ty& operator^=(Ty& _Left, Ty _Right) \
{   /* return _Left ^= _Right */ \
    _Left = (Ty)((std::underlying_type<Ty>::type)_Left ^ (std::underlying_type<Ty>::type)_Right); return (_Left); \
} \
\
inline Ty operator&(Ty _Left, Ty _Right) \
{   /* return _Left & _Right */ \
    return ((Ty)((std::underlying_type<Ty>::type)_Left & (std::underlying_type<Ty>::type)_Right)); \
} \
\
inline Ty operator|(Ty _Left, Ty _Right) \
{   /* return _Left | _Right */ \
    return ((Ty)((std::underlying_type<Ty>::type)_Left | (std::underlying_type<Ty>::type)_Right)); \
} \
\
inline Ty operator^(Ty _Left, Ty _Right) \
{   /* return _Left ^ _Right */ \
    return ((Ty)((std::underlying_type<Ty>::type)_Left ^ (std::underlying_type<Ty>::type)_Right)); \
} \
\
inline Ty operator~(Ty _Left) \
{   /* return ~_Left */ \
    return ((Ty)~(std::underlying_type<Ty>::type)_Left); \
} \
\
inline bool operator!(Ty _Left) \
{   /* return _Left == 0 */ \
    return _Left == static_cast<Ty>(0); \
}
