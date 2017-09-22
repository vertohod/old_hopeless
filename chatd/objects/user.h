#ifndef USERS_H
#define USERS_H

#include "object.h"
#include "types.h"

namespace chatd
{

class user : public blizzard::object, public blizzard::factory<user>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(OID, oid)
    DECLARATION_FIELD(OID, avatar)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(size_t, password)
    DECLARATION_FIELD(time_t, created)
    DECLARATION_FIELD(time_t, block_time)
    DECLARATION_FIELD(size_t, violations_amount) // при N нарушениях блокировать по IP
    DECLARATION_FIELD(bool, block)

//    personal_t          m_personal;
//    system_messages_t   m_sysmess_info;
//    system_messages_t   m_sysmess_error;
//    size_t              m_hash_personal;    
//    size_t              m_hash_image;
//    size_t              m_hash_audio;
//    size_t              m_hash_video;
//    size_t              m_hash_archive;
//    time_t              m_last_access;
//    DECLARATION_FIELD(OID, current_room)

//    boost::circular_buffer_space_optimized<room_mess> m_messages;

public:
    user();

public:

    struct key_nickname : public key_interface
    {
	private:
		sptr_cstr m_nickname;

    public:
        static const std::string m_name;

        key_nickname();
        key_nickname(const object& obj);

        const std::string& name() const override;

        size_t hash() const override;
        bool operator==(const key_interface&) const override;
        bool operator<(const key_interface&) const override;

		void set_nickname(sptr_cstr& nickname);
		sptr_cstr get_nickname();

    private:
        key_nickname(const key_nickname&);
        key_nickname& operator=(const key_nickname&);
    };

};

} // end of namespace

#endif
