// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/**
 * C-API Header
 *
 */

#include <stddef.h>

// toolset
typedef int toolset_t; /// toolset handle type
toolset_t kart_create_default_toolset();
toolset_t kart_create_toolset(const char* config_file_path);
void kart_delete_toolset(toolset_t ts_handle);
const char* kart_get_compiler_options(toolset_t ts_handle);
void kart_set_compiler_options(toolset_t ts_handle, const char* options);
void kart_append_compiler_options(toolset_t ts_handle, const char* options);
const char* kart_get_linker_options(toolset_t ts_handle);
void kart_set_linker_options(toolset_t ts_handle, const char* options);
void kart_append_linker_options(toolset_t ts_handle, const char* options);

// program
typedef int program_t; // program handle type
program_t kart_create_program(const char* src, size_t size);
program_t kart_create_program_from_src_file(const char* file_name);
program_t kart_create_program_from_binary_file(const char* file_name);
void kart_delete_program(program_t prog_handle);
void kart_build_program(program_t prog_handle, toolset_t ts_handle);
const char* kart_get_build_log(program_t prog_handle);
void* kart_get_kernel(program_t prog_handle, const char* name);
const char* kart_get_binary_file_name(program_t prog_handle);


