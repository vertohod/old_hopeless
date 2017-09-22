#include "configure.h"
#include "stream.h"
#include "room.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(room)

DEFINITION_CONSTS(room, "room", false, 1)

DEFINITION_FIELD(room, sptr_cstr, room_name)
DEFINITION_FIELD(room, bool, private)
DEFINITION_FIELD(room, time_t, last_out)
DEFINITION_FIELD(room, int, type)
DEFINITION_FIELD(room, int, transparence)
DEFINITION_FIELD(room, sptr_cstr, background_path)
DEFINITION_FIELD(room, sptr_cstr, background_color)
DEFINITION_FIELD(room, sptr_cstr, textsize)
DEFINITION_FIELD(room, sptr_cstr, description)

room::room() : FIELD(private)(false), FIELD(last_out)(0), FIELD(type)(0), FIELD(transparence)(0) {}

std::unique_ptr<blizzard::keys_stores_t> room::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // Добавляем ключи

    return res;
}

void room::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

std::shared_ptr<blizzard::object> room::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace chatd
