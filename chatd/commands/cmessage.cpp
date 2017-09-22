#include "cmessage.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(cmessage)

DEFINITION_CONSTS(cmessage, "cmessage", true, 1)

DEFINITION_FIELD(cmessage, int, command)
DEFINITION_FIELD(cmessage, OID, poid)
DEFINITION_FIELD(cmessage, OID, room_oid)
DEFINITION_FIELD(cmessage, OID, oid_to)
DEFINITION_FIELD(cmessage, bool, private)
DEFINITION_FIELD(cmessage, sptr_cstr, message)
DEFINITION_FIELD(cmessage, sptr_cstr, raw)
DEFINITION_FIELD(cmessage, bool, encrypted)

cmessage::cmessage() : FIELD(command)(RESULT), FIELD(cmessage)(0), FIELD(request)(0) {}

std::unique_ptr<blizzard::keys_stores_t> cmessage::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // у комманд ключей нет

    return res;
}

void cmessage::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void cmessage::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void cmessage::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> cmessage::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
