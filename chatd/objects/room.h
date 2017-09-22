#ifndef ROOM_H
#define ROOM_H

#include <deque>

#include "types.h"
#include "object.h"
#include "table.h"

namespace chatd
{

class room : public blizzard::object, public blizzard::factory<room>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(sptr_cstr, room_name)
    DECLARATION_FIELD(bool, private)
    DECLARATION_FIELD(time_t, last_out)
    DECLARATION_FIELD(int, type)
    DECLARATION_FIELD(int, transparence)
    DECLARATION_FIELD(sptr_cstr, background_path)
    DECLARATION_FIELD(sptr_cstr, background_color)
    DECLARATION_FIELD(sptr_cstr, textsize)
    DECLARATION_FIELD(sptr_cstr, description)

    // Обязательные методы -----
    static std::unique_ptr<blizzard::keys_stores_t> init_keys();

    virtual void _serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const override;
    virtual std::shared_ptr<blizzard::object> make_copy() const override;
    // -------------------------

public:
    room();
};

} // end of namespace chatd

#endif
