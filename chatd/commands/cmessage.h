#ifndef CMESSAGE_H
#define CMESSAGE_H

#include "commands.h"
#include "object.h"
#include "types.h"

namespace chatd
{

class cmessage : public blizzard::object, public blizzard::factory<cmessage>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(int, command)
    DECLARATION_FIELD(OID, poid)
    DECLARATION_FIELD(OID, room_oid)
    DECLARATION_FIELD(OID, oid_to)
    DECLARATION_FIELD(bool, private)
    DECLARATION_FIELD(sptr_cstr, message)
    DECLARATION_FIELD(sptr_cstr, raw)
    DECLARATION_FIELD(bool, encrypted)

public:
    cmessage();

    // Обязательные методы -----
    static std::unique_ptr<blizzard::keys_stores_t> init_keys();

    virtual void _serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const override;
    virtual void _deserialization(rapidjson::Document& obj) override;
    virtual void _deserialization(rapidjson::Value& obj) override;
    virtual std::shared_ptr<blizzard::object> make_copy() const override;
    // -------------------------
};

} // end of namespace

#endif
