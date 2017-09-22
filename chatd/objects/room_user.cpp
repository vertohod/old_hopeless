#include <boost/functional/hash.hpp>
#include "configure.h"
#include "stream.h"
#include "room_user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(room_user)

DEFINITION_CONSTS(room_user, "room_user", true, 1)

DEFINITION_FIELD(room_user, OID, oid_room)
DEFINITION_FIELD(room_user, OID, oid_user)

room_user::room_user() : FIELD(oid_room)(0), FIELD(oid_user)(0) {}

std::unique_ptr<blizzard::keys_stores_t> room_user::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // Добавляем ключи
    // user
    {
        auto key_store = std::make_shared<blizzard::key_store_umset<const key_user>>();
        res->insert(std::make_pair(key_store->name(), key_store));
    }

    // room_user
    {
        auto key_store = std::make_shared<blizzard::key_store_uset<const key_room_user>>();
        res->insert(std::make_pair(key_store->name(), key_store));
    }

    return res;
}

void room_user::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void room_user::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void room_user::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> room_user::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

std::shared_ptr<blizzard::key_interface> room_user::crete_key_user(OID user_oid)
{
    auto res = std::make_shared<key_user>();
    res->set_user(user_oid);
    return res;
}

std::shared_ptr<blizzard::key_interface> room_user::crete_key_room_user(OID room_oid, OID user_oid)
{
    auto res = std::make_shared<key_room_user>();
    res->set_room(room_oid);
    res->set_user(user_oid);
    return res;
}

// ------------------------ keys -------------------------------------
// USER
const std::string room_user::key_user::key_user::m_name = "user";
room_user::key_user::key_user() : key_interface(0), m_user(0) {}
room_user::key_user::key_user(const object& obj) : key_interface(obj.get_index()), m_user(obj.get_user()) {}
const std::string& room_user::key_user::name() const
{
    return m_name;
}
OID room_user::key_user::get_user() const
{
    return m_user;
}
void room_user::key_user::set_user(OID oid)
{
    m_user = oid;
}
size_t room_user::key_user::hash() const
{
    return std::hash<OID>()(m_user);
}
bool room_user::key_user::operator==(const key_interface& obj) const
{
    return m_user == dynamic_cast<const key_user*>(&obj)->get_user();
}
bool room_user::key_user::operator<(const key_interface& obj) const
{
    return m_user < dynamic_cast<const key_user*>(&obj)->get_user();
}

// ROOM_USER
const std::string room_user::key_room_user::key_room_user::m_name = "room_user";
room_user::key_room_user::key_room_user() : key_interface(0), m_room(0), m_user(0) {}
room_user::key_room_user::key_room_user(const object& obj) : key_interface(obj.get_index()), m_room(obj.get_room()), m_user(obj.get_user()) {}
const std::string& room_user::key_room_user::name() const
{
    return m_name;
}
OID room_user::key_room_user::get_room() const
{
    return m_room;
}
OID room_user::key_room_user::get_user() const
{
    return m_user;
}
void room_user::key_room_user::set_room(OID oid)
{
    m_room = oid;
}
void room_user::key_room_user::set_user(OID oid)
{
    m_user = oid;
}
size_t room_user::key_room_user::hash() const
{
    std::size_t res = 0 ;
    boost::hash_combine(res, m_room);
    boost::hash_combine(res, m_user);
    return res;
}
bool room_user::key_room_user::operator==(const key_interface& obj) const
{
    return m_room == dynamic_cast<const key_room_user*>(&obj)->get_room()
        && m_user == dynamic_cast<const key_room_user*>(&obj)->get_user();
}
bool room_user::key_room_user::operator<(const key_interface& obj) const
{
    if (m_room < dynamic_cast<const key_room_user*>(&obj)->get_room()) return true;
    if (m_room == dynamic_cast<const key_room_user*>(&obj)->get_room()
        && return m_user < dynamic_cast<const key_room_user*>(&obj)->get_user()) return true;

    return false;
}
// -----------------------------------------------------------------------

} // end of namespace chatd
