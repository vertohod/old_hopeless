#include "context.h"

namespace chatd
{

void context::add_response(std::shared_ptr<const object>& response)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_responses.push_back(response);
}

responses_ptr_t context::get_responses()
{
	responses_ptr_t res(new responses_t());

	std::lock_guard<std::mutex> lock(m_mutex);
	res->swap(m_responses);
	return res;
}

} // end of namespace
