#include "change_nickname_request.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(change_nickname_request)

DEFINITION_CONSTS(change_nickname_request, "change_nickname_request", true, 1)

DEFINITION_FIELD(change_nickname_request, int, command)
DEFINITION_FIELD(change_nickname_request, sptr_cstr, nickname)
DEFINITION_FIELD(change_nickname_request, sptr_cstr, password)

change_nickname_request::change_nickname_request() : FIELD(command)(CHANGE_NICKNAME_REQUEST) {}

std::unique_ptr<blizzard::keys_stores_t> change_nickname_request::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // у комманд ключей нет

    return res;
}

void change_nickname_request::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

void change_nickname_request::_deserialization(rapidjson::Document& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

void change_nickname_request::_deserialization(rapidjson::Value& obj)
{
    object::_deserialization(obj);
    for (auto& member : obj.GetObject()) {
        auto it = m_deserialization_functions.find(member.name.GetString());
        if (it != m_deserialization_functions.end()) {
            it->second(this, member.value);
        }
    }
}

std::shared_ptr<blizzard::object> change_nickname_request::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
