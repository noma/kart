// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/log.hpp"

#include "kart/constants.hpp"
#include "kart/kart_error.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace kart {
namespace detail {

std::string make_log_msg_prefix(log_level_t log_level, const std::string& location)
{
	std::ostringstream msg_ss;
	msg_ss << constants::log_msg_prefix << ": " << log_level << ": " << location << ": ";
	return msg_ss.str();
}

// NOTE: internal helper, not in header
std::string make_log_msg(log_level_t log_level, const std::string& location, const std::string& msg)
{
	return make_log_msg_prefix(log_level, location) + msg;
}

// NOTE: internal helper, not in header
// write pre-generated message to std::err
// TODO (maybe): make log channel configurable, i.e. file or std::err
void write_log(log_level_t log_level, const std::string& msg)
{
	const log_level_t current_log_level = get_log_level();

	assert(log_level > log_level_t::none);

	if (log_level <= current_log_level) {
		std::cerr << msg << std::endl;
	}
}

log_level_t get_log_level()
{
	const char* log_level_cstr = std::getenv(constants::env_var_log_level);

	log_level_t log_level = constants::default_log_level;
	if (log_level_cstr) {
		try {
			// parse
			std::istringstream iss;
			iss.str(log_level_cstr);
			iss >> log_level;
		} catch(const log_level_parse_error& e) {
			// silently recover from error
			log_level = constants::default_log_level;
		} catch(...) {
			const auto msg = make_log_msg(log_level_t::error, "kart::detail::get_log_level()", "Caught unexpected exception, terminating.");
			std::cerr << msg << std::endl;
			std::terminate();
		} // try
	} // if

	return log_level;
}

void log(log_level_t log_level, const std::string& location, const std::string& msg)
{
	const auto msg_str = make_log_msg(log_level, location, msg);
	write_log(log_level, msg_str);
}

void log_and_throw(log_level_t log_level, const std::string& location, const std::string& msg)
{
	const auto msg_str = make_log_msg(log_level, location, msg);
	write_log(log_level, msg_str);

	throw kart_error(msg_str);
}

std::ostream& log_stream(log_level_t log_level, const std::string& location) {
	static std::ostream null_ostream(nullptr); // NOTE: swallows all input, badbit is set if anyone checks
	const log_level_t current_log_level = get_log_level();

	if (log_level <= current_log_level) {
		return std::cerr << make_log_msg_prefix(log_level, location);
	} else {
		return null_ostream;
	}
}

} // namespace detail
} // namespace kart