#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <boost/thread/detail/singleton.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <deque>

#include "object.h"
#include "context.h"

// list of commands headers
#include "result.h"
#include "authentication_request.h"
#include "change_nickname_request.h"
#include "logout.h"
#include "remove_login.h"
#include "come_in.h"
#include "come_out.h"
#include "cmessage.h"

namespace chatd
{

class processor
{
private:
	typedef std::deque<std::pair<std::shared<context>, std::shared<blizzard::object>>> commands_t;
    commands_t m_commands;
    std::vector<std::shared<std::thread>> m_threads;
    std::shared_ptr<context> m_context;

    std::mutex m_mutex;

    volatile bool m_stop_flag;

public:
    processor();
    ~processor();

    void start(int count_threads);
    void add_command(std::shared<context>& cnt_ptr, std::shared<blizzard::object>& command_ptr);

private:
    void thread_function();
    void process_commands(commands_t& commands);
    void generate_robot_message(OID room_oid, const std::string& message);

private:
	bool check_nickname(sptr_cstr& nickname);
    void process(context& cnt, authentication_request& command) const;
    void process(context& cnt, change_nickname_request& command) const;
    void process(context& cnt, logout& command) const;
    void process(context& cnt, remove_login& command) const;
    void process(context& cnt, come_in& command) const;
    void process(context& cnt, come_out& command) const;
    void process(context& cnt, cmessage& command) const;
};

inline processor& pr()
{
    return singleton<processor>::instance();
}

} // end of namespace
#endif
