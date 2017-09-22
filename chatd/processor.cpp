#include <unistd.h>

#include "processor.h"

namespace chatd
{

processor::processor() : m_stop_flag(false) {}

processor::~processor()
{
    m_stop_flag = true;

    for (auto& thread : m_threads) thread->join();
}

processor::start(int count_threads)
{
    m_stop_flag = false;

    for (int count = 0; count < count_threads; ++count) {
        m_threads.push_back(std::shared_ptr<std::thread>(new std::thread(std::bind(&processor::thread_function, this))));
    }
}

void processor::thread_function()
{
    size_t count_empty = 0;
    static const useconds_t pause_min = 32000;
    static const useconds_t pause_max = 512000;

    useconds_t pause = 0;

    while (!m_stop_flag) {

        for (useconds_t pause_temp = 0; pause_temp < pause; pause_temp += pause_min) {
            if (m_stop_flag) return;
            ::usleep(pause_min);
        }

        commands_t commands_temp;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_commands.size() != 0) {
                commands_temp.swap(m_commands);
                pause = 0;
            } else {
                pause = (pause == 0) ? pause_min : (pause * 2);
                if (pause > pause_max) pause = pause_max;
                continue;
            }
        }

        process_commands(commands_temp);
    }
}

void processor::add_command(std::shared<context>& cnt_ptr, std::shared<blizzard::object>& command_ptr)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_commands.push_back(std::make_pair(cnt_ptr, command_ptr));
}

void processor::process_commands(commands_t& commands)
{
    for (auto& command_pr : commands) {

		auto& cnt = *command_pr.first;

        switch (command_pr.second->get_command()) {
            case AUTHENTICATION_REQUEST:
                auto& command = *dynamic_cast<authentication_request*>(command_pr.second.get());
                process(cnt, command);
                break;
            case CHANGE_NICKNAME_REQUEST:
                auto& command = *dynamic_cast<change_nickname_request*>(command_pr.second.get());
                process(cnt, command);
                break;
            case LOGOUT:
                auto& command = *dynamic_cast<logout*>(command_pr.second.get());
                process(cnt, command);
                break;
            case REMOVE_LOGIN:
                auto& command = *dynamic_cast<remove_login*>(command_pr.second.get());
                process(cnt, command);
                break;
            case COME_IN:
                auto& command = *dynamic_cast<come_in*>(command_pr.second.get());
                process(cnt, command);
                break;
            case COME_OUT:
                auto& command = *dynamic_cast<get_off*>(command_pr.second.get());
                process(cnt, command);
                break;
            case CMESSAGE:
                auto& command = *dynamic_cast<cmessage*>(command_pr.second.get());
                process(cnt, command);
                break;
            default:
                lo::l(lo::ERROR) << "Unknown command: " << command_ptr->name();
                break;
        }
    }
}

bool processor::check_nickname(sptr_cstr& nickname)
{
    static boost::regex reg(std::string("^[") + ALPH_ALL + "]*$");

	auto nickname_length = util::length_utf8(*nickname);

    return (nickname_length >= cfg().nickname_min && nickname_length <= cfg().nickname_max);
}

void process::generate_robot_message(OID room_oid, const std::string& message)
{
    auto message = chatd::cmessage::create();

    message->set_poid(0);
    message->set_room_oid(room_oid);
    message->set_oid_to(0);
    message->set_private(false);
    message->set_message(message);
    message->set_raw(message);
    message->set_encrypted(false);

    add_command(m_context, message);
}

void processor::process(context& cnt, authentication_request& command) const
{
	auto res = chatd::result::create();
	res->set_request(command.get_index());

    if (check_nickname(command.get_nickname()) {

        if (!boost::regex_match(*command.get_nickname(), reg)) {

			res->set_result(SMESS111_INVALID_CHARACTERS);
			res->set_description(smessage(SMESS111_INVALID_CHARACTERS));
			cnt.add_response(res);

            return;
        }

		auto user_key = std::make_shared<chatd::user::key_nickname>()
		user_key->set_nickname(command.get_nickname());
		auto res = bl().find<chatd::user>(user_key);

		if (res->size() == 1) {

			auto user = bl().get<chatd::user>(res->at(0));

			if (user->get_password() != 0 && user->get_password() == std::hash(*command.get_password())) {

				cnt.user = user;

				res->set_result(SMESS000_OK);
				res->set_description(smessage(SMESS000_OK));

			} else {

				if (user->get_password() == 0) {

					res->set_result(SMESS105_NICKNAME_EXIST);
					res->set_description(smessage(SMESS105_NICKNAME_EXIST));

				} else {

					res->set_result(SMESS116_PASSWORD_WRONG);
					res->set_description(smessage(SMESS116_PASSWORD_WRONG));

				}

			}

		} else {

			auto user = chatd::user::create();
			user->set_nickname(command.get_nickname());

			if (!command.get_password()->empty()) {
				user->set_password(std::hash(*command.get_password()));
			}

			auto oid = bl().insert(user);

			if (oid == 0) {

				res->set_result(SMESS001_ERROR);
				res->set_description(smessage(SMESS001_ERROR));

			} else {

				cnt.user = user;

				res->set_result(SMESS000_OK);
				res->set_description(smessage(SMESS000_OK));

			}
		}

    } else {

		res->set_result(SMESS104_NICKNAME_WRONG);
		res->set_description(smessage(SMESS104_NICKNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));

	}

	cnt.add_response(res);
}

void processor::process(context& cnt, change_nickname_request& command) const
{
	auto res = chatd::result::create();
	res->set_request(command.get_index());

    if (check_nickname(command.get_nickname()) {

		chatd::user::key_nickname key;
		key.set_nickname(command.get_nickname());

		auto user_ptr = bl().find<chatd::user>(key);
		if (user_ptr->size() == 0) {

			auto user_copy = cnt.user->make_copy();
			user_copy->set_nickname(command.get_nickname());
			if (!bl().update(user_copy)) {

				res->set_result(SMESS001_ERROR);
				res->set_description(smessage(SMESS001_ERROR));

			}
		}

	} else {

		res->set_result(SMESS104_NICKNAME_WRONG);
		res->set_description(smessage(SMESS104_NICKNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));

	}

	cnt.add_response(res);
}

void processor::process(context& cnt, logout& command) const
{
    cnt.user.reset();
}

void processor::process(context& cnt, remove_login& command) const
{
	auto res = chatd::result::create();
	res->set_request(command.get_index());

    if (bl().remove(cnt.user)) {
        res->set_result(SMESS000_OK);
        res->set_description(smessage(SMESS000_OK));
    } else {
        res->set_result(SMESS001_ERROR);
        res->set_description(smessage(SMESS001_ERROR));
    }

	cnt.add_response(res);
}

void processor::process(context& cnt, come_in& command) const
{
    auto room_user = chatd::room_user::create();
    room_user->set_room_oid(command.get_room_oid());
    room_user->set_user_oid(cnt.user->get_oid());

    auto res = bl().insert(room_user);

    if (res != 0) {
        generate_robot_message(command.get_room_oid(), smessage(SMESS101_USER_CAME_IN, cnt.user->get_nickname()));
    }
}

void processor::process(context& cnt, get_off& command) const
{
    // TODO
}

void processor::process(context& cnt, cmessage& command) const
{
    auto message = chatd::message::create();
    message->set_poid(command.get_poid());
    message->set_room_oid(command.get_room_oid());
    message->set_oid_from(cnt.user->get_oid());
    message->set_oid_to(command.get_oid_to());
    message->set_private(command.get_private());
    message->set_time(util::get_gtime());
    message->set_color(util::get_color());
    message->set_avatar(cnt.user->get_avatar());
    message->set_message(command.get_message());
    message->set_raw(command.get_raw());
    message->set_encrypted(command.get_encrypted());

    if (command.get_oid_to() != 0) {
        auto user_to = bl().get<chatd::user>(command.get_oid_to());
        message->set_name_to(user_to->get_nickname());
    }

    message->set_name_from(cnt.user->get_nickname());

    auto message_oid = bl().insert(message);
}

} // end of namespace
