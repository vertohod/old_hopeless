#include "configure.h"
#include "stream.h"
#include "room_message.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(room_message)

DEFINITION_CONSTS(room_message, "room_message", true, 1)

DEFINITION_FIELD(room_message, OID, oid_room)
DEFINITION_FIELD(room_message, OID, oid_message)

room_message::room_message() : FIELD(oid_room)(0), FIELD(oid_message)(0) {}

std::unique_ptr<blizzard::keys_stores_t> room_message::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // Добавляем ключи

    return res;
}

void room_message::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void room_message::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void room_message::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> room_message::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace chatd
