#ifndef AUTHENTICATION_REQUEST_H
#define AUTHENTICATION_REQUEST_H

#include "commands.h"
#include "object.h"
#include "types.h"

namespace chatd
{

class authentication_request : public blizzard::object, public blizzard::factory<authentication_request>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(int, command)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(sptr_cstr, password)

public:
    authentication_request();

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
