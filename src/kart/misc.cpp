// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/misc.hpp"

#include "kart/log.hpp"

#include <iostream>

namespace kart {

bool check_error(const bs::error_code& ec, const std::string& location, const std::string& msg)
{
	if (ec)
	{
		std::ostringstream msg_stream;
		msg_stream << msg << ": " << ec.message();

		detail::log_and_throw(detail::log_level_t::error, location, msg_stream.str());
		return true;
	}
	return false;
}

} // namespace kart
