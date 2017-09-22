#ifndef RESULT_H
#define RESULT_H

#include "commands.h"
#include "object.h"
#include "types.h"

namespace chatd
{

class result : public blizzard::object, public blizzard::factory<result>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(int, command)
    DECLARATION_FIELD(int, result)
    DECLARATION_FIELD(sptr_cstr, description)
    DECLARATION_FIELD(OID, request)

public:
    result();

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
