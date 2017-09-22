#include <stdexcept>
#include <ctime>

#include "object.h"
#include "format.h"
#include "stream.h"
#include "util.h"

namespace blizzard
{

// обязательно добно быть выше чем определение полей DEFINITION_FIELD
object::serialization_functions_t object::m_serialization_functions;
object::deserialization_functions_t object::m_deserialization_functions;

DEFINITION_FIELD(object, unsigned char, event)
DEFINITION_FIELD(object, time_t, time)
DEFINITION_FIELD(object, OID, index)
DEFINITION_FIELD(object, OID, owner)

object::create_functions_t& object::instance_store_functions()
{
    static object::create_functions_t m_create_functions;
    return m_create_functions;
}

void object::registration_object(const std::string& name, std::function<std::shared_ptr<object>()> func)
{
    instance_store_functions().insert(std::make_pair(name, func));
}

std::shared_ptr<object> object::create_object(const std::string& name)
{
    auto& functions = instance_store_functions();
    auto it = functions.find(name);

    if (it != functions.end()) {
        return it->second();
    } else {
        throw std::runtime_error("Unknown object: " + name);
    }    

    return std::shared_ptr<object>();
}

object::object() :
    FIELD(time)(0), FIELD(event)(object::NONE), FIELD(owner)(0), FIELD(index)(0)
{}

object::~object() {}

size_t object::hash() const
{
    return std::hash<OID>()(FIELD(index));
}

bool object::operator==(const object& obj)
{
    return FIELD(index) == obj.FIELD(index);
}

bool object::operator<(const object& obj)
{
    return FIELD(index) < obj.FIELD(index);
}

std::unique_ptr<const key_interface> object::get_key(const std::string& name)
{
    // этот класс содержит только один ключ
    // производные могут содержать больше
    if (name == object::key_index::m_name) {

        return std::unique_ptr<key_interface>(new key_index(*this));

    } else throw std::runtime_error("Unknown name of key: " + name);

    // эта строка никогда не сработает
    return std::unique_ptr<key_interface>(new key_index(*this));
}

uptr_str object::serialization() const
{
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Value obj(rapidjson::kObjectType);

    _serialization(obj, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    obj.Accept(writer);

    return uptr_str(new std::string(buffer.GetString()));
}

void object::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void object::deserialization(const std::string& json)
{
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    _deserialization(doc);
}

void object::_deserialization(rapidjson::Document& obj)
{
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void object::_deserialization(rapidjson::Value& obj)
{
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

uptr_cstr object::get_field(const std::string& json, const std::string& field_name)
{
	rapidjson::Document doc;

	try {
		doc.Parse(json.c_str());
	} catch (const std::exception& e) {
		throw std::runtime_error(std::string("Can't parse json, ") + e.what() + ", string: " + json);
	}

	if (doc.HasMember(field_name.c_str()) && doc[field_name.c_str()].IsString()) {
		return doc[field_name.c_str()].GetString();
	}

    return "";
}

bool object::operator==(const object& obj) const
{
    return FIELD(index) == obj.FIELD(index);
}


// ----------------------------- KEYS ---------------------------------


key_interface::key_interface(OID index) : m_index(index) {}
key_interface::~key_interface() {}
OID key_interface::get_index() const { return m_index; }
void key_interface::set_index(OID index) { m_index = index; }
// INDEX
const std::string object::key_index::m_name = "index";
object::key_index::key_index() : key_interface(0) {}
object::key_index::key_index(const object& obj) : key_interface(obj.get_index()) {}
const std::string& object::key_index::name() const
{
    return m_name;
}
size_t object::key_index::hash() const
{
    return std::hash<OID>()(get_index());
}
bool object::key_index::operator==(const key_interface& obj) const
{
    return get_index() == obj.get_index();
}
bool object::key_index::operator<(const key_interface& obj) const
{
    return get_index() < obj.get_index();
}

// OWNER
const std::string object::key_owner::key_owner::m_name = "owner";
object::key_owner::key_owner() : key_interface(0), m_owner(0) {}
object::key_owner::key_owner(const object& obj) : key_interface(obj.get_index()), m_owner(obj.get_owner()) {}
const std::string& object::key_owner::name() const
{
    return m_name;
}
OID object::key_owner::get_owner() const
{
    return m_owner;
}
void object::key_owner::set_owner(OID oid)
{
    m_owner = oid;
}
size_t object::key_owner::hash() const
{
    return std::hash<OID>()(m_owner);
}
bool object::key_owner::operator==(const key_interface& obj) const
{
    return m_owner == dynamic_cast<const key_owner*>(&obj)->get_owner();
}
bool object::key_owner::operator<(const key_interface& obj) const
{
    return m_owner < dynamic_cast<const key_owner*>(&obj)->get_owner();
}
// -----------------------------------------------------------------------
} // end of namespace
