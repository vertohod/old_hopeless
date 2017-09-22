#ifndef AUDIO_H
#define AUDIO_H

#include "object.h"
#include "types.h"

namespace chatd
{

class audio : public blizzard::object, public blizzard::factory<audio>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(sptr_cstr, title)
    DECLARATION_FIELD(sptr_cstr, artist)
    DECLARATION_FIELD(sptr_cstr, extension)
    DECLARATION_FIELD(sptr_cstr, content_type)

public:
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
