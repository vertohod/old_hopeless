#ifndef ROOM_MESSAGE_H
#define ROOM_MESSAGE_H

#include <deque>

#include "types.h"
#include "object.h"
#include "table.h"

namespace chatd
{

class room_message : public blizzard::object, public blizzard::factory<room_message>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(OID, oid_room)
    DECLARATION_FIELD(OID, oid_message)

    // Обязательные методы -----
    static std::unique_ptr<blizzard::keys_stores_t> init_keys();

    virtual void _serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const override;
    virtual void _deserialization(rapidjson::Document& obj) override;
    virtual void _deserialization(rapidjson::Value& obj) override;
    virtual std::shared_ptr<blizzard::object> make_copy() const override;
    // -------------------------

public:
    room_message();
};

} // end of namespace chatd

#endif
