#pragma once

namespace Json {
    class Value;
    enum ValueType;
}

namespace utils {
    class jsonLib final
    {
    public:
        static bool ParseStringToJson(const std::string& strValue, Json::Value& jsonValue) noexcept;
        static Json::Value ParseStringToJson(const std::string& strValue);

        static bool WriteJsonToString(const Json::Value& jsonValue, std::string& strValue, const uint32_t decimalPoint = DEFAULT_POINT) noexcept;
        static std::string WriteJsonToString(const Json::Value& jsonValue, const uint32_t decimalPoint = DEFAULT_POINT);

        static bool IsMember(const Json::Value& value, const std::string& sMember);
        static bool IsMember(const Json::Value& value, const std::string& sMember, const Json::ValueType valueType);

        static bool GetString(const Json::Value& value, const std::string& sMember, std::string& sMemberValue);
        static std::string GetString(const Json::Value& value, const std::string& sMember);
        static std::string GetStringDefault(const Json::Value& value, const std::string& sMember, const std::string& sDefault);
        //0x1234 or 1234 -> return int
        static std::uint32_t GetInterTypeString(const Json::Value& value, const std::string& sMember, const std::uint32_t nDefault = 0);

        static bool GetBoolean(const Json::Value& value, const std::string& sMember, bool& bMemberValue);
        static bool GetBoolean(const Json::Value& value, const std::string& sMember);
        static bool GetBooleanDefault(const Json::Value& value, const std::string& sMember, const bool bDefault);

        static bool GetInt32(const Json::Value& value, const std::string& sMember, int32_t& nMemberValue);
        static bool GetInt32(const Json::Value& value, const std::string& sMember, int32_t& nMemberValue, const int32_t nMinimum, const int32_t nMaximum);
        static int32_t GetInt32(const Json::Value& value, const std::string& sMember);
        static int32_t GetInt32(const Json::Value& value, const std::string& sMember, const int32_t nMinimum, const int32_t nMaximum);
        static int32_t GetInt32Default(const Json::Value& value, const std::string& sMember, const int32_t nDefault);
        static int32_t GetInt32Default(const Json::Value& value, const std::string& sMember, const int32_t nDefault, const int32_t nMinimum, const int32_t nMaximum);

        static bool GetUInt32(const Json::Value& value, const std::string& sMember, uint32_t& nMemberValue);
        static bool GetUInt32(const Json::Value& value, const std::string& sMember, uint32_t& nMemberValue, const uint32_t nMinimum, const uint32_t nMaximum);
        static uint32_t GetUInt32(const Json::Value& value, const std::string& sMember);
        static uint32_t GetUInt32(const Json::Value& value, const std::string& sMember, const uint32_t nMinimum, const uint32_t nMaximum);
        static uint32_t GetUInt32Default(const Json::Value& value, const std::string& sMember, const uint32_t nDefault);
        static uint32_t GetUInt32Default(const Json::Value& value, const std::string& sMember, const uint32_t nDefault, const uint32_t nMinimum, const uint32_t nMaximum);

        static bool GetFloat(const Json::Value& value, const std::string& sMember, float& fMemberValue);
        static float GetFloat(const Json::Value& value, const std::string& sMember);
        static float GetFloatDefault(const Json::Value& value, const std::string& sMember, const float fDefault);

        static bool GetDouble(const Json::Value& value, const std::string& sMember, double& dMemberValue);
        static double GetDouble(const Json::Value& value, const std::string& sMember);
        static double GetDoubleDefault(const Json::Value& value, const std::string& sMember, const double dDefault);

    protected:
        static const uint32_t DEFAULT_POINT;
    };//jsonLib
}//namespace utils
