// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_log_level_hpp
#define kart_log_level_hpp

#include <map>
#include <stdexcept>
#include <string>

namespace kart {
namespace detail {

enum class log_level_t {
	none = 0,  // show nothing
	error = 1, // show only errors
	warning = 2,  // show errors and warnings
	info = 3 // show errors, warnings, and information messsages
};

extern const std::map<log_level_t, std::string> log_level_names;

std::ostream& operator<<(std::ostream& out, const log_level_t& log_level);
std::istream& operator>>(std::istream& in, log_level_t& log_level);

class log_level_parse_error : public std::runtime_error
{
public:
	log_level_parse_error(const std::string& msg) : runtime_error(msg) {};
};

} // namespace detail
} // namespace kart

#endif // kart_log_level_hpp