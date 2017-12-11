// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_constants_hpp
#define kart_constants_hpp

#include "kart/log_level.hpp"

namespace kart {
namespace constants {

extern const char* env_var_default_toolset_config;
extern const char* env_var_tmp_path;
extern const char* env_var_keep_tmp_files;
extern const char* env_var_log_level;

extern const detail::log_level_t default_log_level;
extern const char* log_msg_prefix;

} // namespace constants
} // namespace kart

#endif // kart_constants_hpp