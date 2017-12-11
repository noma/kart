// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_log_hpp
#define kart_log_hpp

#include "kart/log_level.hpp"

#include <ostream>

namespace kart {
namespace detail {

/**
 * Reads the current log level from the environment.
 * See kart/log_level.hpp for the allowed values.
 *
 * @return current log level
 */
log_level_t get_log_level();

void log(log_level_t log_level, const std::string& location, const std::string& msg);
void log_and_throw(log_level_t log_level, const std::string& location, const std::string& msg);

std::ostream& log_stream(log_level_t log_level, const std::string& location);

} // namespace detail
} // namespace kart

#endif // kart_log_hpp