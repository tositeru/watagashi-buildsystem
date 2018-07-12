#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/variant.hpp>
#include <boost/utility/string_view.hpp>

#include "../exception.hpp"

namespace parser
{

struct Enviroment;
class ErrorHandle;
class IScope;

struct NoneValue
{};

struct MemberDefined;
struct ObjectDefined
{
    std::string name;
    std::unordered_map<std::string, MemberDefined> members;

    ObjectDefined();
    ObjectDefined(std::string const& name);
};

struct Value;
struct Object
{
    std::unordered_map<std::string, Value> members;
    ObjectDefined const* pDefined;

    Object(ObjectDefined const* pDefined);

    bool applyObjectDefined();

};

struct Reference
{
    Enviroment const* pEnv;
    std::list<std::string> nestName;
    Reference(Enviroment const* pEnv, std::list<std::string> const& nestName);
    Value const* ref()const;
};

struct Value
{
    enum class Type
    {
        None,
        Bool,
        String,
        Number,
        Array,
        Object,
        ObjectDefined,
        MemberDefined,
        Reference,
    };

    using string = std::string;
    using number = double;
    using array = std::vector<Value>;
    using object = Object;

    static Value const none;
    static Value const emptyStr;
    static ObjectDefined const arrayDefined; // dummy objectDefined
    static ObjectDefined const objectDefined;
    static boost::string_view toString(Type type);
    static Type toType(boost::string_view const& str);

    Type type;
    struct InnerData;
    std::unique_ptr<InnerData> pData;

    Value();
    Value(Value const& right);
    Value(Value &&right);
    Value& operator=(Value const& right);
    Value& operator=(Value &&right);
    ~Value();

    Value(NoneValue const& right);
    Value(bool const& right);
    Value(string const& right);
    Value(number const& right);
    Value(array const& right);
    Value(object const& right);
    Value(ObjectDefined const& right);
    Value(MemberDefined const& right);
    Value(Reference const& right);

    Value(NoneValue && right);
    Value(bool && right);
    Value(string && right);
    Value(number && right);
    Value(array && right);
    Value(object && right);
    Value(ObjectDefined && right);
    Value(MemberDefined && right);
    Value(Reference && right);

    Value& init(Type type_);

    template<typename T>
    Value& operator=(T const& right)
    {
        Value tmp(right);
        *this = std::move(tmp);
        return *this;
    }

    template<typename T>
    Value& operator=(T&& right)
    {
        Value tmp(std::move(right));
        *this = std::move(tmp);
        return *this;
    }

    bool operator==(Value const& right)const;
    bool operator!=(Value const& right)const;
    bool operator<(Value const& right)const;
    bool operator<=(Value const& right)const;
    bool operator>(Value const& right)const;
    bool operator>=(Value const& right)const;

    void pushValue(Value const& pushValue);
    bool addMember(IScope const& member);
    bool addMember(std::string const&name, Value const& value);
    void appendStr(boost::string_view const& strView);

    std::string toString()const;

    template<typename T> T& get()
    {
        return boost::get<T>(this->pData->data);
    }

    template<typename T> T const& get()const {
        return boost::get<T>(this->pData->data);
    }

    bool isExsitChild(std::string const& name)const;

    Value& getChild(std::string const& name);
    Value const& getChild(std::string const& name)const;
};

struct MemberDefined
{
    Value::Type type;
    Value defaultValue;

    MemberDefined() = default;
    MemberDefined(Value::Type type, Value const& defaultValue);
    MemberDefined(Value::Type type, Value && defaultValue);
};

struct Value::InnerData
{
    // TODO use std::variant after all
    boost::variant<
        NoneValue,
        bool,
        string,
        number,
        array,
        object,
        ObjectDefined,
        MemberDefined,
        Reference> data;

    InnerData()
        : data(NoneValue())
    {}

    InnerData(InnerData const& right)
        : data(right.data)
    {}

    InnerData(InnerData && right)
        // TODO use std::variant after all
        : data(right.data)
    {}

    template<typename T>
    InnerData(T const& right)
        : data(right)
    {}

    template<typename T>
    InnerData(T&& right)
    {
        // TODO use std::variant after all
        this->data = right;
    }
};

}