#include "authentication_request.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(authentication_request)

DEFINITION_CONSTS(authentication_request, "authentication_request", true, 1)

DEFINITION_FIELD(authentication_request, int, command)
DEFINITION_FIELD(authentication_request, sptr_cstr, nickname)
DEFINITION_FIELD(authentication_request, sptr_cstr, password)

authentication_request::authentication_request() : FIELD(command)(AUTHENTICATION_REQUEST) {}

std::unique_ptr<blizzard::keys_stores_t> authentication_request::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // у комманд ключей нет

    return res;
}

void authentication_request::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void authentication_request::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void authentication_request::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> authentication_request::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
