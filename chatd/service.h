#ifndef SERVICE_H
#define SERVICE_H

#include "http_server.h"
#include "context.h"
#include "types.h"

class service : public net::http_server
{
public:
    service(net::boost_socket_ptr socket);

private:
    void handle_process(const request& req);

    // методы обработки конкретных запросов пользователя
    void fill_context(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void send(std::shared_ptr<context>context& cnt cnt_ptr);
    void login(std::shared_ptr<context>context& cnt cnt_ptr);
    void logout(std::shared_ptr<context>context& cnt cnt_ptr);
    void comein(std::shared_ptr<context>context& cnt cnt_ptr, OID room);
    void getout(std::shared_ptr<context>context& cnt cnt_ptr);
    void change(std::shared_ptr<context>context& cnt cnt_ptr);
    void upload(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void remove(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void image(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void audio(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void video(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void archive(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void personal(std::shared_ptr<context>context& cnt cnt_ptr);
    void add(const request&, std::shared_ptr<context>context& cnt cnt_ptr);

    void activate(const request& req);
    void upload_image(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void upload_audio(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void upload_video(const request& req, std::shared_ptr<context>context& cnt cnt_ptr);
    void upload_file(const request& req, std::shared_ptr<context>context& cnt cnt_ptr);
    void settings(const request& req, std::shared_ptr<context>context& cnt cnt_ptr);

    void remove_images(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_audios(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_videos(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_archives(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_room_allows(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_personal_messages(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void remove_password(std::shared_ptr<context>context& cnt cnt_ptr);
    void remove_alias(std::shared_ptr<context>context& cnt cnt_ptr);

    void add_alias(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void add_room_allows(std::shared_ptr<context>context& cnt cnt_ptr, sptr_cvec oids);
    void add_room(const request&, std::shared_ptr<context>context& cnt cnt_ptr);
    void add_password(const request&, context&);
    void add_avatar(const request&, context&);
    void add_invite(const request&, context&);
    void invite(const request& req, std::shared_ptr<context>context& cnt cnt_ptr);

    bool check_message(std::shared_ptr<context>context& cnt cnt_ptr);
    void generate_answer(std::shared_ptr<context>context& cnt cnt_ptr);
    void generate_short_answer(std::shared_ptr<context>context& cnt cnt_ptr);
};

#endif
