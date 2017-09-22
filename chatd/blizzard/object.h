#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_map>
#include <stdexcept>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "types.h"
#include "json.h"

namespace blizzard
{

//--------------------
#define DESER(OBJECT, TYPE, NAME) \
[](blizzard::object* ptr, rapidjson::Value& obj)\
{\
    dynamic_cast<OBJECT*>(ptr)->set_##NAME(blizzard::convert_json_object<TYPE>(obj));\
}


//--------------------
#define SER(OBJECT, NAME) \
[](const blizzard::object* ptr, rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al){blizzard::add_member(obj, al, #NAME, dynamic_cast<const OBJECT*>(ptr)->get_##NAME());}


//--------------------
#define ADD_FUNCTION(OBJECT, TYPE, NAME) \
struct add_function_into_map_##OBJECT##_##NAME\
{\
    add_function_into_map_##OBJECT##_##NAME()\
    {\
        OBJECT::m_serialization_functions.push_back(SER(OBJECT, NAME));\
        OBJECT::m_deserialization_functions.insert(std::make_pair(#NAME, DESER(OBJECT, TYPE, NAME)));\
    }\
};\
add_function_into_map_##OBJECT##_##NAME object_for_add_function_##OBJECT##_##NAME;

// ------ добавление полей в объекты -----------------------------------------------------------
#define DECLARATION_FIELD(TYPE, NAME) \
private:\
    TYPE m_field_##NAME;\
public:\
    TYPE get_##NAME() const;\
    void set_##NAME(const TYPE& val);


//--------------------
#define DEFINITION_FIELD(OBJECT, TYPE, NAME) \
TYPE OBJECT::get_##NAME() const\
{\
    return m_field_##NAME;\
}\
void OBJECT::set_##NAME(const TYPE& val)\
{\
    m_field_##NAME = val;\
}\
ADD_FUNCTION(OBJECT, TYPE, NAME)


//--------------------
#define FIELD(NAME) m_field_##NAME


//--------------------
#define DECLARATION_CONSTS \
private:\
    static const std::string m_name;\
    static const bool m_increment;\
    static const int m_version;\
public:\
    const std::string& get_name() const;\
    int get_version() const;\
    void set_name(const std::string&);\
    void set_version(int);\
    static const std::string& sname();\
    static bool sincrement();\
    virtual const std::string& name() const override;\
    virtual int version() const override;


//--------------------
#define DEFINITION_CONSTS(OBJECT, NAME, INCREMENT, VERSION) \
const std::string OBJECT::m_name = NAME;\
const bool OBJECT::m_increment = INCREMENT;\
const int OBJECT::m_version = VERSION;\
const std::string& OBJECT::get_name() const {return m_name;}\
int OBJECT::get_version() const {return m_version;}\
void OBJECT::set_name(const std::string& val){}\
void OBJECT::set_version(int val){}\
const std::string& OBJECT::sname() {return OBJECT::m_name;}\
bool OBJECT::sincrement() {return OBJECT::m_increment;}\
const std::string& OBJECT::name() const {return OBJECT::m_name;}\
int OBJECT::version() const {return OBJECT::m_version;}\
ADD_FUNCTION(OBJECT, const std::string, name)\
ADD_FUNCTION(OBJECT, int, version)


//--------------------
#define DECLARATION_FIELDS_FUNCTIONS_STORES \
static deserialization_functions_t m_deserialization_functions;\
static serialization_functions_t m_serialization_functions;


//--------------------
#define DEFINITION_FIELDS_FUNCTIONS_STORES(OBJECT) \
OBJECT::deserialization_functions_t OBJECT::m_deserialization_functions;\
OBJECT::serialization_functions_t OBJECT::m_serialization_functions;\
struct add_function_create_for_##OBJECT\
{\
    add_function_create_for_##OBJECT()\
    {\
        blizzard::object::registration_object(#OBJECT, &OBJECT::create_interface);\
    }\
};\
add_function_create_for_##OBJECT object_add_function_create_##OBJECT;


// ---------------------------------------------------------------------------------------------

// дружественный класс для object
template <typename T> class table;

#define SEPARATOR "\t"

struct key_interface
{
private:
    OID m_index;

public:
    key_interface(OID index);
    virtual ~key_interface();
    OID get_index() const;
    void set_index(OID);

    virtual const std::string& name() const = 0;
    virtual size_t hash() const = 0;
    virtual bool operator==(const key_interface&) const = 0;
    virtual bool operator<(const key_interface&) const = 0;
};

class object
{
protected:
    typedef std::unordered_map<std::string, std::function<void(object*, rapidjson::Value& obj)>> deserialization_functions_t;
    typedef std::vector<std::function<void(const object*, rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al)>> serialization_functions_t;
    typedef std::unordered_map<std::string, std::function<std::shared_ptr<object>()>> create_functions_t;

    static create_functions_t m_create_functions;

public:
    static serialization_functions_t m_serialization_functions;
    static deserialization_functions_t m_deserialization_functions;

public:
    enum EVENT {
        NONE    = 0,
        SELECT  = 1,
        INSERT  = 2,
        UPDATE  = 3,
        REMOVE  = 4,
        RESULT  = 5
    };

    // указатель на контейнейр с индексами
    // указательно на мьютекс таблицы

private:
    DECLARATION_FIELD(unsigned char, event)
    DECLARATION_FIELD(time_t, time)
    DECLARATION_FIELD(OID, index)
    DECLARATION_FIELD(OID, owner)

private:
    static create_functions_t& instance_store_functions();

public:
    object();
    virtual ~object();

    virtual const std::string& name() const = 0;
    virtual int version() const = 0;
    virtual std::shared_ptr<object> make_copy() const = 0;

    static void registration_object(const std::string& name, std::function<std::shared_ptr<object>()> func);
    static std::shared_ptr<object> create_object(const std::string& name);

    // дня хранения в таблице
    size_t hash() const;
    bool operator==(const object&);
    bool operator<(const object&);
    // --------------------

    virtual std::unique_ptr<const key_interface> get_key(const std::string& name);
    virtual uptr_str serialization() const final;
    virtual void _serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const;
    virtual void deserialization(const std::string& json) final;
    virtual void _deserialization(rapidjson::Document& obj);
    virtual void _deserialization(rapidjson::Value& obj);

public:
    static uptr_cstr get_field(const std::string& json, const std::string& field_name);

public:
    bool operator==(const object& obj) const;

public:
    // Пример ключа.
    // этот может пригодиться, если понадобится упорядочить по index
    struct key_index : public key_interface
    {
    public:
        static const std::string m_name;

        key_index();
        key_index(const object& obj);

        const std::string& name() const override;

        size_t hash() const override;
        bool operator==(const key_interface&) const override;
        bool operator<(const key_interface&) const override;

    private:
        key_index(const key_index&);
        key_index& operator=(const key_index&);
    };

    struct key_owner : public key_interface
    {
    private:
        OID    m_owner;

    public:
        static const std::string m_name;

        key_owner();
        key_owner(const object& obj);

        const std::string& name() const override;

        OID get_owner() const;
        void set_owner(OID);

        size_t hash() const override;
        bool operator==(const key_interface&) const override;
        bool operator<(const key_interface&) const override;

    private:
        key_owner(const key_owner&);
        key_owner& operator=(const key_owner&);
    };
};

template <typename T>
class factory
{
public:
    // здесь возвращается shared_ptr вместо unique_ptr, т.к. потом удобней использовать
    static std::shared_ptr<T> create()
    {
        return std::shared_ptr<T>(new T());
    }

    static std::shared_ptr<blizzard::object> create_interface()
    {
        return std::shared_ptr<blizzard::object>(new T());
    }
};

template <typename T>
inline void add_member(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const T& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);
    rapidjson::Value val(fld);
    obj.AddMember(key, val, al);
}

template <>
inline void add_member<std::string>(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const std::string& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);
    rapidjson::Value val(fld.c_str(), fld.length(), al);
    obj.AddMember(key, val, al);
}

template <>
inline void add_member<sptr_cstr>(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const sptr_cstr& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);
    rapidjson::Value val(fld->c_str(), fld->length(), al);
    obj.AddMember(key, val, al);
}

template <>
inline void add_member<std::shared_ptr<std::vector<OID>>>(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const std::shared_ptr<std::vector<OID>>& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);

    rapidjson::Value arr(rapidjson::kArrayType);
    for (auto val : *fld) arr.PushBack(val, al);

    obj.AddMember(key, arr, al);
}

template <>
inline void add_member<std::shared_ptr<std::vector<sptr_str>>>(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const std::shared_ptr<std::vector<sptr_str>>& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);

    rapidjson::Value arr(rapidjson::kArrayType);
    for (auto& ptr : *fld) {
        rapidjson::Value val(ptr->c_str(), ptr->length(), al);
        arr.PushBack(val, al);
    }

    obj.AddMember(key, arr, al);
}

template <>
inline void add_member<std::shared_ptr<blizzard::object>>(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al, const std::string& name, const std::shared_ptr<blizzard::object>& fld)
{
    rapidjson::Value key(name.c_str(), name.length(), al);

    rapidjson::Value obj_inserted(rapidjson::kObjectType);
    fld->_serialization(obj_inserted, al);

    obj.AddMember(key, obj_inserted, al);
}

template <typename T>
inline T convert_json_object(rapidjson::Value& obj)
{
    if (obj.IsUint()) {
        auto val = obj.GetUint();
        return static_cast<T>(val);
    } else if (obj.IsInt()) {
        auto val = obj.GetInt();
        return static_cast<T>(val);
    } else if (obj.IsUint64()) {
        auto val = obj.GetUint64();
        return static_cast<T>(val);
    } else if (obj.IsInt64()) {
        auto val = obj.GetInt64();
        return static_cast<T>(val);
    } else if (obj.IsDouble()) {
        auto val = obj.GetDouble();
        return static_cast<T>(val);
    } else if (obj.IsBool()) {
        auto val = obj.GetBool();
        return static_cast<T>(val);
    } else {
        throw std::runtime_error("Unknown type (1)");
    }

    return T();
}

template <>
inline std::string convert_json_object<std::string>(rapidjson::Value& obj)
{
    if (obj.IsString()) {
        return obj.GetString();
    } else {
        throw std::runtime_error("Unknown type (2)");
    }

    return std::string();
}

template <>
inline const std::string convert_json_object<const std::string>(rapidjson::Value& obj)
{
    if (obj.IsString()) {
        return obj.GetString();
    } else {
        throw std::runtime_error("Unknown type (3)");
    }

    return std::string();
}

template <>
inline sptr_cstr convert_json_object<sptr_cstr>(rapidjson::Value& obj)
{
    if (obj.IsString()) {
        return obj.GetString();
    } else {
        throw std::runtime_error("Unknown type (4)");
    }

    return sptr_cstr();
}

template <>
inline std::shared_ptr<std::vector<OID>> convert_json_object<std::shared_ptr<std::vector<OID>>>(rapidjson::Value& obj)
{
	auto res = std::make_shared<std::vector<OID>>();

	if (obj.IsArray()) {
		for (auto& val : obj.GetArray()) res->push_back(val.GetUint64());
	} else {
        throw std::runtime_error("Unknown type (5)");
	}

    return res;
}

template <>
inline std::shared_ptr<std::vector<sptr_str>> convert_json_object<std::shared_ptr<std::vector<sptr_str>>>(rapidjson::Value& obj)
{
	auto res = std::make_shared<std::vector<sptr_str>>();

	if (obj.IsArray()) {
		for (auto& val : obj.GetArray()) res->push_back(val.GetString());
	} else {
        throw std::runtime_error("Unknown type (6)");
	}

    return res;
}

} // end of namespace
#endif
