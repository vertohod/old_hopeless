#ifndef SESSION_H
#define SESSION_H

#include <memory>

#include "object.h"
#include "types.h"

namespace chatd
{

class session : public blizzard::object, public blizzard::factory<session>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(std::shared_ptr<const UUID>, uuid)

private:
    static std::shared_ptr<const UUID> gen(const std::string& arg_uuid_str);
    static std::shared_ptr<const UUID> gen_random();

};

}
#endif
