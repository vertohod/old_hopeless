#include "result.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(result)

DEFINITION_CONSTS(result, "result", true, 1)

DEFINITION_FIELD(result, int, command)
DEFINITION_FIELD(result, int, result)
DEFINITION_FIELD(result, sptr_cstr, description)
DEFINITION_FIELD(result, OID, request)

result::result() : FIELD(command)(RESULT), FIELD(result)(0), FIELD(request)(0) {}

std::unique_ptr<blizzard::keys_stores_t> result::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // у комманд ключей нет

    return res;
}

void result::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void result::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void result::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> result::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
