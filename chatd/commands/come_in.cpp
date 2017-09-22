#include "come_in.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(come_in)

DEFINITION_CONSTS(come_in, "come_in", true, 1)

DEFINITION_FIELD(come_in, int, command)
DEFINITION_FIELD(come_in, OID, room_oid)

come_in::come_in() : FIELD(command)(RESULT), FIELD(come_in)(0), FIELD(request)(0) {}

std::unique_ptr<blizzard::keys_stores_t> come_in::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // у комманд ключей нет

    return res;
}

void come_in::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void come_in::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void come_in::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> come_in::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
