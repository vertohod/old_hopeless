#ifndef ROOM_USER_H
#define ROOM_USER_H

#include <deque>

#include "types.h"
#include "object.h"
#include "table.h"

namespace chatd
{

class room_user : public blizzard::object, public blizzard::factory<room_user>
{
public:
    DECLARATION_FIELDS_FUNCTIONS_STORES

private:
    DECLARATION_CONSTS

    DECLARATION_FIELD(OID, room_oid)
    DECLARATION_FIELD(OID, user_oid)

    // Обязательные методы -----
    static std::unique_ptr<blizzard::keys_stores_t> init_keys();

    virtual void _serialization(rapidjson::Value& obj, rapidjson::MemoryPoolAllocator<>& al) const override;
    virtual void _deserialization(rapidjson::Document& obj) override;
    virtual void _deserialization(rapidjson::Value& obj) override;
    virtual std::shared_ptr<blizzard::object> make_copy() const override;
    // -------------------------

public:
    std::shared_ptr<blizzard::key_interface> crete_key_user(OID user_oid);
    std::shared_ptr<blizzard::key_interface> crete_key_room_user(OID room_oid, OID user_oid);

public:
    room_user();

    // -------------- kyes ---------------
    struct key_user : public blizzard::key_interface
    {
    private:
        OID m_user;

    public:
        static const std::string m_name;

        key_user();
        key_user(const object& obj);

        const std::string& name() const override;

        OID get_user() const;
        void set_user(OID);

        size_t hash() const override;
        bool operator==(const key_interface&) const override;
        bool operator<(const key_interface&) const override;

    private:
        key_user(const key_user&);
        key_user& operator=(const key_user&);
    };

    struct key_room_user : public blizzard::key_interface
    {
    private:
        OID m_room;
        OID m_user;

    public:
        static const std::string m_name;

        key_room_user();
        key_room_user(const object& obj);

        const std::string& name() const override;

        OID get_room() const;
        OID get_user() const;
        void set_room(OID);
        void set_user(OID);

        size_t hash() const override;
        bool operator==(const key_interface&) const override;
        bool operator<(const key_interface&) const override;

    private:
        key_room_user(const key_room_user&);
        key_room_user& operator=(const key_room_user&);
    };
};

} // end of namespace chatd

#endif
