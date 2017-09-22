#include "logout.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(logout)

DEFINITION_CONSTS(logout, "logout", true, 1)

DEFINITION_FIELD(logout, int, command)

logout::logout() : FIELD(command)(LOGOUT) {}

std::unique_ptr<blizzard::keys_stores_t> logout::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // Добавляем ключи
    // owner
/*
    {
        auto key_store = std::make_shared<blizzard::key_store_umset<const blizzard::object::key_owner>>();
        res->insert(std::make_pair(key_store->name(), key_store));
    }
*/

    return res;
}

void logout::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void logout::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void logout::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> logout::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
