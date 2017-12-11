// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/**
 * C-API implementation, also used for FORTRAN bindings.
 *
 * Uses opaque integer handles that are keys into maps with C++ API objects.
 */

#include <iostream>
#include <map>
#include "kart/kart.hpp"

extern "C" {
#include "kart/kart.h"
}

namespace kart {
namespace c_api {

using toolset_map = std::map<toolset_t, kart::toolset>;
using program_map = std::map<program_t, kart::program>;

toolset_t get_toolset_handle()
{
	static int toolset_handle_counter = 0;
	return ++toolset_handle_counter; // first handle is 1
}

program_t get_program_handle()
{
	static int program_handle_counter = 0;
	return ++program_handle_counter; // first handle is 1
}

toolset_map& get_toolset_map()
{
	static toolset_map ts_map;
	return ts_map;
}

program_map& get_program_map()
{
	static program_map prog_map;
	return prog_map;
}

toolset_map::iterator find_toolset_handle(toolset_t ts_handle, const std::string& hint)
{
	toolset_map& tsm = get_toolset_map();
	auto ts_it = tsm.find(ts_handle);
	if (ts_it == std::end(tsm)) {
		std::cerr << "KART error: invalid toolset handle, " << hint << std::endl;
		std::terminate(); // FAIL hard
	}
	return ts_it;
}

program_map::iterator find_program_handle(program_t prog_handle, const std::string& hint)
{
	program_map& pm = get_program_map();
	auto prog_it = pm.find(prog_handle);
	if (prog_it == std::end(pm)) {
		std::cerr << "KART error: invalid program handle, " << hint << std::endl;
		std::terminate(); // FAIL hard
	}
	return prog_it;
}

} // namespace c_api
} // namespace kart

/* external C-API: */
extern "C" {

// default toolset ctor
toolset_t kart_create_default_toolset()
{
	using namespace kart::c_api;
	toolset_map& tsm = get_toolset_map();
	toolset_t handle = get_toolset_handle();
	try {
		tsm.insert(std::make_pair(handle, kart::toolset()));
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
	return handle;
}
// toolset ctor that takes config_file_path
toolset_t kart_create_toolset(const char* config_file_path)
{
	using namespace kart::c_api;
	toolset_map& tsm = get_toolset_map();
	toolset_t handle = get_toolset_handle();
	try {
		tsm.insert(std::make_pair(handle, kart::toolset(config_file_path)));
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
	return handle;
}

void kart_delete_toolset(toolset_t ts_handle)
{
	using namespace kart::c_api;
	toolset_map& tsm = get_toolset_map();
	tsm.erase(find_toolset_handle(ts_handle, "delete_toolset()"));
}

const char* kart_get_compiler_options(toolset_t ts_handle)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "get_compiler_options()");
	return ts_it->second.compiler_options().c_str();
}

void kart_set_compiler_options(toolset_t ts_handle, const char* options)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "set_compiler_options()");
	return ts_it->second.set_compiler_options(options);
}

void kart_append_compiler_options(toolset_t ts_handle, const char* options)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "append_compiler_options()");
	return ts_it->second.append_compiler_options(options);
}

const char* kart_get_linker_options(toolset_t ts_handle)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "get_linker_options()");
	return ts_it->second.linker_options().c_str();
}

void kart_set_linker_options(toolset_t ts_handle, const char* options)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "set_linker_options()");
	return ts_it->second.set_linker_options(options);
}

void kart_append_linker_options(toolset_t ts_handle, const char* options)
{
	using namespace kart::c_api;
	auto ts_it = find_toolset_handle(ts_handle, "append_linker_option()");
	return ts_it->second.append_linker_options(options);
}


// program ctor
program_t kart_create_program(const char* src, size_t size)
{
	using namespace kart::c_api;
	program_map& pm = get_program_map();
	program_t handle = get_program_handle();
	try {
		pm.insert(std::make_pair(handle, kart::program(src, size)));
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
	return handle;
}

program_t kart_create_program_from_src_file(const char* file_name)
{
	using namespace kart::c_api;
	program_map& pm = get_program_map();
	program_t handle = get_program_handle();
	try {
		pm.insert(std::make_pair(handle, kart::program::create_from_src_file(file_name)));
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
	return handle;
}

program_t kart_create_program_from_binary_file(const char* file_name)
{
	using namespace kart::c_api;
	program_map& pm = get_program_map();
	program_t handle = get_program_handle();
	try {
		pm.insert(std::make_pair(handle, kart::program::create_from_binary_file(file_name)));
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
	return handle;
}

void kart_delete_program(program_t prog_handle)
{
	using namespace kart::c_api;
	program_map& pm = get_program_map();
	pm.erase(find_program_handle(prog_handle, "delete_program()"));
}

void kart_build_program(program_t prog_handle, toolset_t ts_handle)
{
	using namespace kart::c_api;
	auto prog_it = find_program_handle(prog_handle, "build_program()");
	auto ts_it = find_toolset_handle(ts_handle, "build_program()");
	try {
		prog_it->second.build(ts_it->second);
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
}

const char* kart_get_build_log(program_t prog_handle)
{
	using namespace kart::c_api;
	auto prog_it = find_program_handle(prog_handle, "get_build_log()");
	return prog_it->second.get_build_log().c_str();
}

void* kart_get_kernel(program_t prog_handle, const char* name)
{
	using namespace kart::c_api;
	auto prog_it = find_program_handle(prog_handle, "get_kernel()");
	try {
		return prog_it->second.get_kernel(name);
	} catch (const kart::kart_error& e) {
		std::cerr << "KART: caught kart_error: " << e.what() << std::endl;
		std::terminate();
	} catch (...) {
		std::cerr << "KART: caught non-kart exception." << std::endl;
		std::terminate();
	}
}

const char* kart_get_binary_file_name(program_t prog_handle)
{
	using namespace kart::c_api;
	auto prog_it = find_program_handle(prog_handle, "binary_file_name()");
	return prog_it->second.binary_file_name().c_str();
}

} // extern "C"
