#include "precompile.h"
#include <json/json.h>
using namespace utils;

    const uint32_t jsonLib::DEFAULT_POINT = std::numeric_limits<uint32_t>::max();

bool jsonLib::ParseStringToJson(const std::string& strValue, Json::Value& jsonValue) noexcept
{
    try
    {
        jsonValue = std::move(ParseStringToJson(strValue));
    } catch (const std::exception&)
    {
        return false;
    }

    return true;
}

Json::Value jsonLib::ParseStringToJson(const std::string& strValue)
{
    Json::Value jsonValue;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    Json::String errorMessage;
    if (reader->parse(strValue.c_str(), strValue.c_str() + strValue.size(), &jsonValue, &errorMessage) == false)
        throw Json::Exception(errorMessage);

    return std::move(jsonValue);
}

bool jsonLib::WriteJsonToString(const Json::Value& jsonValue, std::string& strValue, const uint32_t decimalPoint /*= DEFAULT_POINT*/) noexcept
{
    try
    {
        strValue = std::move(WriteJsonToString(jsonValue, decimalPoint));
    } catch (const std::exception&)
    {
        return false;
    }
    return true;
}

std::string jsonLib::WriteJsonToString(const Json::Value& jsonValue, const uint32_t decimalPoint /*= DEFAULT_POINT*/)
{
    Json::StreamWriterBuilder JsonWriter;
    if (decimalPoint != DEFAULT_POINT)
    {
        JsonWriter.settings_["precision"] = decimalPoint;
        JsonWriter.settings_["precisionType"] = "decimal";
    }
    return std::move(Json::writeString(JsonWriter, jsonValue));
}

bool jsonLib::IsMember(const Json::Value& value, const std::string& sMember)
{
    if (!value.isObject())
        return false; // ?ㅻ툕?앺듃媛 ?꾨떂
    if (!value.isMember(sMember))
        return false; // ?대떦 硫ㅻ쾭媛 ?놁쓬

    return true;
}

bool jsonLib::IsMember(const Json::Value& value, const std::string& sMember, const Json::ValueType valueType)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];

    switch (valueType)
    {
        case Json::nullValue:
            return member.isNull();
        case Json::intValue:
            return member.isInt64();
        case Json::uintValue:
            return member.isUInt64();
        case Json::realValue:
            return member.isDouble();
        case Json::stringValue:
            return member.isString();
        case Json::booleanValue:
            return member.isBool();
        case Json::arrayValue:
            return member.isArray();
        case Json::objectValue:
            return member.isObject();
        default:
            return false;
    }
}

bool jsonLib::GetString(const Json::Value& value, const std::string& sMember, std::string& sMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isConvertibleTo(Json::stringValue))
        return false;

    sMemberValue = member.asString();
    return true;
}

std::string jsonLib::GetString(const Json::Value& value, const std::string& sMember)
{
    std::string sResult;
    if (false == GetString(value, sMember, sResult))
        throw std::logic_error("value is not string");

    return sResult;
}

std::string jsonLib::GetStringDefault(const Json::Value& value, const std::string& sMember, const std::string& sDefault)
{
    std::string sResult;
    if (false == GetString(value, sMember, sResult))
        return sDefault;

    return sResult;
}

std::uint32_t jsonLib::GetInterTypeString(const Json::Value& value, const std::string& sMember, const std::uint32_t nDefault)
{
    uint32_t nResult = nDefault;
    std::string strValue;
    if (false == GetString(value, sMember, strValue))
        return nResult;

    if (true == strValue.empty())
        return nResult;
    //https://en.cppreference.com/w/cpp/string/byte/strtol
    //Base媛 0?멸꼍?? 0x/0X/0/decimal auto detected
    nResult = static_cast<uint32_t>(std::strtoll(strValue.c_str(), nullptr, 0));
    return nResult;
}

bool jsonLib::GetBoolean(const Json::Value& value, const std::string& sMember, bool& bMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isConvertibleTo(Json::booleanValue))
        return false;

    bMemberValue = member.asBool();
    return true;
}

bool jsonLib::GetBoolean(const Json::Value& value, const std::string& sMember)
{
    bool bResult;
    if (false == GetBoolean(value, sMember, bResult))
        throw std::logic_error("value is not boolean");

    return bResult;
}

bool jsonLib::GetBooleanDefault(const Json::Value& value, const std::string& sMember, const bool bDefault)
{
    bool bResult;
    if (false == GetBoolean(value, sMember, bResult))
        return bDefault;

    return bResult;
}

bool jsonLib::GetInt32(const Json::Value& value, const std::string& sMember, int32_t& nMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isInt())
        return false;

    nMemberValue = member.asInt();
    return true;
}

bool jsonLib::GetInt32(const Json::Value& value, const std::string& sMember, int32_t& nMemberValue, const int32_t nMinimum, const int32_t nMaximum)
{
    int32_t nResult;
    if (false == GetInt32(value, sMember, nResult))
        return false;

    if (nResult < nMinimum || nMaximum < nResult)
        return false; // 吏?뺣맂 踰붿쐞瑜? 踰쀬뼱??

    nMemberValue = nResult;
    return true;
}

int32_t jsonLib::GetInt32(const Json::Value& value, const std::string& sMember)
{
    int32_t nResult;
    if (false == GetInt32(value, sMember, nResult))
        throw std::logic_error("value is not int32");

    return nResult;
}

int32_t jsonLib::GetInt32(const Json::Value& value, const std::string& sMember, const int32_t nMinimum, const int32_t nMaximum)
{
    int32_t nResult;
    if (false == GetInt32(value, sMember, nResult, nMinimum, nMaximum))
        throw std::logic_error("value is not int32");

    return nResult;
}

int32_t jsonLib::GetInt32Default(const Json::Value& value, const std::string& sMember, const int32_t nDefault)
{
    int32_t nResult;
    if (false == GetInt32(value, sMember, nResult))
        return nDefault;

    return nResult;
}

int32_t jsonLib::GetInt32Default(const Json::Value& value, const std::string& sMember, const int32_t nDefault, const int32_t nMinimum, const int32_t nMaximum)
{
    int32_t nResult;
    if (false == GetInt32(value, sMember, nResult, nMinimum, nMaximum))
        return nDefault;

    return nResult;
}

bool jsonLib::GetUInt32(const Json::Value& value, const std::string& sMember, uint32_t& nMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isUInt())
        return false;

    nMemberValue = member.asUInt();
    return true;
}

bool jsonLib::GetUInt32(const Json::Value& value, const std::string& sMember, uint32_t& nMemberValue, const uint32_t nMinimum, const uint32_t nMaximum)
{
    uint32_t nResult;
    if (false == GetUInt32(value, sMember, nResult))
        return false;

    if (nResult < nMinimum || nMaximum < nResult)
        return false; // 吏?뺣맂 踰붿쐞瑜? 踰쀬뼱??

    nMemberValue = nResult;
    return true;
}

uint32_t jsonLib::GetUInt32(const Json::Value& value, const std::string& sMember)
{
    uint32_t nResult;
    if (false == GetUInt32(value, sMember, nResult))
        throw std::logic_error("value is not uint32");

    return nResult;
}

uint32_t jsonLib::GetUInt32(const Json::Value& value, const std::string& sMember, const uint32_t nMinimum, const uint32_t nMaximum)
{
    uint32_t nResult;
    if (false == GetUInt32(value, sMember, nResult, nMinimum, nMaximum))
        throw std::logic_error("value is not uint32");

    return nResult;
}

uint32_t jsonLib::GetUInt32Default(const Json::Value& value, const std::string& sMember, const uint32_t nDefault)
{
    uint32_t nResult;
    if (false == GetUInt32(value, sMember, nResult))
        return nDefault;

    return nResult;
}

uint32_t jsonLib::GetUInt32Default(const Json::Value& value, const std::string& sMember, const uint32_t nDefault, const uint32_t nMinimum, const uint32_t nMaximum)
{
    uint32_t nResult;
    if (false == GetUInt32(value, sMember, nResult, nMinimum, nMaximum))
        return nDefault;

    return nResult;
}

bool jsonLib::GetFloat(const Json::Value& value, const std::string& sMember, float& fMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isDouble())
        return false;

    fMemberValue = member.asFloat();
    return true;
}

float jsonLib::GetFloat(const Json::Value& value, const std::string& sMember)
{
    float fResult = 0.0f;
    if (false == GetFloat(value, sMember, fResult))
        throw std::logic_error("value is not float");

    return fResult;
}

float jsonLib::GetFloatDefault(const Json::Value& value, const std::string& sMember, const float fDefault)
{
    float fResult = 0.0f;
    if (false == GetFloat(value, sMember, fResult))
        return fDefault;

    return fResult;
}

bool jsonLib::GetDouble(const Json::Value& value, const std::string& sMember, double& dMemberValue)
{
    if (!IsMember(value, sMember))
        return false;

    const Json::Value& member = value[sMember];
    if (!member.isDouble())
        return false;

    dMemberValue = member.asDouble();
    return true;
}

double jsonLib::GetDouble(const Json::Value& value, const std::string& sMember)
{
    double dResult = 0.0;
    if (false == GetDouble(value, sMember, dResult))
        throw std::logic_error("value is not float");

    return dResult;
}

double jsonLib::GetDoubleDefault(const Json::Value& value, const std::string& sMember, const double dDefault)
{
    double dResult = 0.0;
    if (false == GetDouble(value, sMember, dResult))
        return dDefault;

    return dResult;
}
