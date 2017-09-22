#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <memory>
#include <deque>
#include <mutex>

#include "object.h"
#include "types.h"
#include "user.h"

namespace chatd
{

class context
{
private:
	std::mutex m_mutex;

public:
    std::shared_ptr<const UUID>					uuid;
    std::shared_ptr<const chat::user>			user;

private:
    std::deque<std::shared_ptr<const blizzard::object>> m_responses;

	typedef std::deque<std::shared_ptr<const blizzard::object>> responses_t;
	typedef std::unique_ptr<responses_t> responses_ptr_t;

public:
	void add_response(std::shared_ptr<const object>& response);
	responses_ptr_t get_responses();

};

} // end of namespace

#endif
