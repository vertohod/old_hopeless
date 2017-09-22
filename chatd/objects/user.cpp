#include "user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(user)

DEFINITION_CONSTS(user, "user", false, 1)

DEFINITION_FIELD(user, OID, oid)
DEFINITION_FIELD(user, OID, avatar)
DEFINITION_FIELD(user, sptr_cstr, nickname)
DEFINITION_FIELD(user, size_t, password)
DEFINITION_FIELD(user, time_t, created)
DEFINITION_FIELD(user, time_t, block_time)
DEFINITION_FIELD(user, size_t, violations_amount)
DEFINITION_FIELD(user, bool, block)

user::user() : 
    FIELD(oid)(0),
    FIELD(avatar)(0),
    FIELD(password)(0),
    FIELD(block_time)(0),
    FIELD(violations_amount)(0),
    FIELD(block)(false)
{
    FIELD(created)(time(NULL));
}

const std::string user::key_nickname::m_name = "nickname";
user::key_nickname::key_nickname() : key_interface(0) {}
const std::string& key_nickname::name() const
{
	return m_name;
}
size_t user::key_nickname::hash() const
{
	return std::hash<std::string>(*m_nick_name);
}
bool user::key_nickname::operator==(const key_interface& obj) const override
{
	return *m_nickname == *dynamic_cast<const key_nickname*>(&obj)->get_nickname();
}
bool user::key_nickname::operator<(const key_interface& obj) const override
{
	return *m_nickname < *dynamic_cast<const key_nickname*>(&obj)->get_nickname();
}
void user::key_nickname::set_nickname(sptr_cstr& nickname)
{
	m_nickname = nickname;
}
sptr_cstr user::key_nickname::get_nickname()
{
	return m_nickname;
}

} // end of namespace
