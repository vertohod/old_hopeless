#include <sys/stat.h>
#include "service_messages.h"
#include "stat_daemon.h"
#include "audio_tags.h"
#include "configure.h"
#include "audio.h"
#include "error.h"
#include "util.h"
#include "log.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(audio)

DEFINITION_CONSTS(audio, "audio", true, 1)

DEFINITION_FIELD(audio, sptr_cstr, title)
DEFINITION_FIELD(audio, sptr_cstr, artist)
DEFINITION_FIELD(audio, sptr_cstr, extension)
DEFINITION_FIELD(audio, sptr_cstr, content_type)

std::unique_ptr<blizzard::keys_stores_t> audio::init_keys()
{
    std::unique_ptr<blizzard::keys_stores_t> res(new blizzard::keys_stores_t());

    // Добавляем ключи
    // owner
    {
        auto key_store = std::make_shared<blizzard::key_store_umset<const blizzard::object::key_owner>>();
        res->insert(std::make_pair(key_store->name(), key_store));
    }

    return res;
}

void audio::_serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const
{
    object::_serialization(obj, al);
    for (auto& func : m_serialization_functions) func(this, obj, al);
}

std::shared_ptr<blizzard::object> audio::make_copy() const
{
    std::shared_ptr<blizzard::object> obj_new = create();

    auto json_ptr = this->serialization();
    obj_new->deserialization(*json_ptr);

    return obj_new;
}

} // end of namespace
