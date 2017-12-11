// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/log_level.hpp"

#include <sstream>

namespace kart {
namespace detail {

const std::map<log_level_t, std::string> log_level_names {
	{ log_level_t::none, "none" },
	{ log_level_t::error, "error" },
	{ log_level_t::warning, "warning" },
	{ log_level_t::info, "info" }
};

std::ostream& operator<<(std::ostream& out, const log_level_t& log_level)
{
	out << log_level_names.at(log_level);
	return out;
}

std::istream& operator>>(std::istream& in, log_level_t& log_level)
{
	std::string value;
	std::getline(in, value);

	// get key to value
	// NOTE: we trust filtering_strategy_names to be complete here
	bool found = false;
	// NOTE: iterate through list of pairs <log_level, std::string>, denoted by <first,second>
	for (auto it = log_level_names.begin(); it != log_level_names.end(); ++it)
		if (it->second == value) {
			log_level = it->first;
			found = true;
			break;
		}

	if (!found) {
		std::stringstream names_ss;
		for (auto it = log_level_names.begin(); it != log_level_names.end(); ++it) {
			if(it != log_level_names.begin())
				names_ss << ", ";
			names_ss << it->second;
		}
		throw log_level_parse_error("'" + value + "' is not a valid log_level, allowed strategies: " + names_ss.str());
	}

	return in;
}

} // namespace detail
} // namespace kart