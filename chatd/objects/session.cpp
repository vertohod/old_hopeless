#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "session.h"

namespace chatd
{

size_t std::hash<UUID>::operator()(const UUID& obj) const
{
    return boost::hash<UUID>()(obj);
}

size_t std::hash<std::shared_ptr<const UUID>>::operator()(std::shared_ptr<const UUID> obj) const
{
    return boost::hash<UUID>()(*obj);
}

size_t std::hash<std::shared_ptr_cstr>::operator()(const std::shared_ptr_cstr& obj) const
{
    return std::hash<std::string>()(*obj);
}


std::shared_ptr<const UUID> session::gen(const std::string& arg_uuid_str)
{
    return std::shared_ptr<const UUID>(new UUID(boost::uuids::string_generator()(arg_uuid_str)));
}

std::shared_ptr<const UUID> session::gen_random()
{
    return std::shared_ptr<const UUID>(new UUID(boost::uuids::random_generator()()));
}

DEFINITION_FIELDS_FUNCTIONS_STORES(session)

DEFINITION_CONSTS(session, "session", true, 1)

DEFINITION_FIELD(session, OID, user_oid)
DEFINITION_FIELD(session, std::shared_ptr<const UUID>, uuid)

} // end of namespace
