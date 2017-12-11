// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/constants.hpp"

namespace kart {
namespace constants {

const char* env_var_default_toolset_config = "KART_DEFAULT_TOOLSET_CONFIG";
const char* env_var_tmp_path = "KART_TMP_PATH";
const char* env_var_keep_tmp_files = "KART_KEEP_TMP_FILES";
const char* env_var_log_level = "KART_LOG_LEVEL";

const detail::log_level_t default_log_level = detail::log_level_t::error;
const char* log_msg_prefix = "KART";

} // namespace constants
} // namespace kart