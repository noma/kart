// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/program.hpp"

#include "kart/constants.hpp"
#include "kart/log.hpp"
#include "kart/misc.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dlfcn.h>

#include <boost/process.hpp>


namespace kart {

namespace bp = ::boost::process;
namespace bfs = ::boost::filesystem;
namespace bs = ::boost::system;
namespace bios = ::boost::iostreams;


program::program(const char* src, size_t size)
{
	bs::error_code error;

	//bfs::path cwd = bfs::current_path(); // get current working directory

	// create a temporary directory with a file containing the source code
	const char* tmp_env = std::getenv(constants::env_var_tmp_path);
	if (tmp_env) {
		tmp_path_ = tmp_env;
	}
	else {
		tmp_path_ = bfs::temp_directory_path(error); // querry system's tmp dir
		check_error(error, "program::program(), bfs::temp_directory_path()", "call returned with");
		tmp_path_ /= bfs::unique_path("%%%%-%%%%-%%%%-%%%%", error); // generate some path name
	}

	detail::log_stream(detail::log_level_t::info, "program::program(...)")
		<< "Using tmp_path: " << tmp_path_ << std::endl;


	if (!bfs::exists(tmp_path_)) {
		detail::log_stream(detail::log_level_t::info, "program::program()")
			<< "Creating temporary directory: " << tmp_path_.string() << std::endl;
		bfs::create_directory(tmp_path_, error); // create temporary directory
		check_error(error, "program::program(), bfs::create_directory()", "call returned with");
	} else { // path exists
		if (!bfs::is_directory(tmp_path_))
			detail::log_and_throw(detail::log_level_t::error, "program::program()", std::string { "Existing temporary path is not a directory: " } + tmp_path_.string());
	}

	bfs::path src_path = tmp_path_ / bfs::unique_path("%%%%-%%%%-%%%%-%%%%.src", error); // generate path for temporary source file
	check_error(error, "program::program(), bfs::unique_path()", "call returned with");
	detail::log_stream(detail::log_level_t::info, "program::program(...)")
		<< "src_path = " << src_path << std::endl;

	std::ofstream src_file(src_path.string()); // open file for writing the source code into

	src_file.write(src, size); // write source into file
	src_file.close(); // explicit close to enforce write to disk

	src_paths_.push_back(src_path);
}

program::program(const std::string& src)
	: program(src.c_str(), src.size())
{ }

program::~program()
{
	// close library
	if (lib_open_) {
		detail::log_stream(detail::log_level_t::info, "program::~program()")
			<< "closing lib_handle: " << lib_handle_ << std::endl;
		dlclose(lib_handle_);
	}

	// delete temporary files, if env_var_keep_tmp_files is not set
	if (!std::getenv(constants::env_var_keep_tmp_files)) {
		auto remove_and_log = [&](const bfs::path& path) {
			static bs::error_code error;
			detail::log_stream(detail::log_level_t::info, "program::~program()")
				<< "Removing path: " << path << std::endl;
			bfs::remove(path, error);
			try {
				check_error(error, "program::~program(), bfs::remove()", "Failed to delete file: " + path.string());
			} catch (...) {
				detail::log_stream(detail::log_level_t::error, "program::~program()")
					<< "Failed to remove: " << path << std::endl;
			}
		};

		// src copies
		for(auto& src_path : src_paths_) {
			remove_and_log(src_path);
		}
		// obj files
		for(auto& obj_path : obj_paths_) {
			remove_and_log(obj_path);
		}
		// lib file
		remove_and_log(lib_path_);

		if (bfs::is_empty(tmp_path_)) {
			remove_and_log(tmp_path_);
		} else {
			detail::log_stream(detail::log_level_t::info, "program::~program()")
				<< "Temporary directory not empty (compiler-generated files?), not deleting, path: " << tmp_path_ << std::endl;
		}
	} // if ! keep tmp
}

void program::set_binary_file_name(const std::string& file_name)
{
	if (built_) {
		detail::log_stream(detail::log_level_t::info, "program::set_binary_file_name()")
			<< "Setting binary file name to: " << file_name << std::endl;
		return;
	} else {
		lib_path_ = file_name;
	}
}

program program::create_from_src_file(const std::string& file_name)
{
	// read in file, and call the src-string-ctor
	std::ifstream src_file(file_name);
	std::stringstream buffer;
	buffer << src_file.rdbuf();
	return program(buffer.str());
}

program program::create_from_binary_file(const std::string& file_name)
{
	program prog; // default constructed program, not usable
	prog.set_lib_path(file_name);
	return prog;
}

void program::build(const toolset& ts)
{
	if (built_) {
		// TODO (feature): allow rebuild with different options,
		//                 this might invalidate function pointers already out there
		//                 only safe with registered kernel pointers
		detail::log_stream(detail::log_level_t::info, "program::build()")
			<< "warning called build on an already built program, doing nothing" << std::endl;
		return;
	}

	bs::error_code error;
	if (lib_path_.empty()) { // check if lib_path wasn't already set externally
		lib_path_ = tmp_path_ / bfs::unique_path("lib%%%%-%%%%-%%%%-%%%%.so", error); // NOTE: unique name seems to needed, even if path is unique for loading multiple libs without conflict
		check_error(error, "program::program(), bfs::unique_path()", "call returned with");
	}
	std::stringstream cmd_line;

	// TODO (feature): complete support for multiple source files
	// compile each source file
	for (auto& src_path : src_paths_) {
		// rename source file to extension defined in toolset (as required by some compilers)
		auto old_path = src_path;
		src_path.replace_extension(ts.source_file_extension());
		bfs::rename(old_path, src_path);
	
		obj_paths_.push_back(src_path);
		obj_paths_.back().replace_extension("o");
		log_paths_.push_back(src_path);
		log_paths_.back().replace_extension("log");

		// TODO (maybe): would be more efficient, to keep fix part and add only varying stuff
		cmd_line << ts.compiler_exe() << " " // compiler
			     << ts.compiler_options_always() << " " // mandatory options
			     << ts.compiler_options() << " " // default options
			     << "-o " << obj_paths_.back() // output
			     << " " << bfs::absolute(src_path);

		// TODO (maybe): parallel build: collect all childs and wait for all after the loop
		detail::log_stream(detail::log_level_t::info, "program::build()")
			<< "compiling with command line: " << cmd_line.str() << std::endl;
		bios::file_descriptor_sink log_file(log_paths_.back());
		bp::child c = bp::execute(bp::initializers::run_exe(ts.compiler_exe()),
			                      bp::initializers::set_cmd_line(cmd_line.str()),
			                      bp::initializers::inherit_env(),
			                      bp::initializers::bind_stderr(log_file),
			                      bp::initializers::bind_stdout(log_file),
		                          bp::initializers::set_on_error(error));
		check_error(error, "program::build(), bp::execute()", "failed in compile step for file '" + bfs::absolute(src_path).string() + "'");

		int status = bp::wait_for_exit(c, error);
		check_error(error, "program::build(), bp::wait_for_exit()", "failed in compile step for file '" + bfs::absolute(src_path).string() + "'");

		// NOTE: both check_error calls seem to not catch compilation failure, so as a workaround we use the returned status code
		status = WEXITSTATUS(status); // NOTE: POSIX, see: man 2 waitpid
		if (status != 0) // fail
			detail::log_and_throw(detail::log_level_t::info, "program::build()", "Compilation for file '" + src_path.string() + "' failed, see build log.");

		cmd_line.clear(); // reset status bits
		cmd_line.str(std::string()); // reset content
	} // for src_paths

	// link step
	cmd_line << ts.linker_exe() << " " // compiler
	         << ts.linker_options_always() << " " // mandatory options
	         << ts.linker_options() << " " // mandatory options
	         << "-o " << bfs::absolute(lib_path_); // default options
	         
	for (auto& obj_path : obj_paths_) {
		cmd_line << " " << bfs::absolute(obj_path); // add all object files
	}

	detail::log_stream(detail::log_level_t::info, "program::build()")
		<< "linking with command line: " << cmd_line.str() << std::endl;
	link_log_path_ = lib_path_;
	link_log_path_.replace_extension("log");
	bios::file_descriptor_sink log_file(link_log_path_);
	bp::child c = bp::execute(bp::initializers::run_exe(ts.linker_exe()),
	                          bp::initializers::set_cmd_line(cmd_line.str()),
	                          bp::initializers::inherit_env(),
	                          bp::initializers::set_on_error(error),
	                          bp::initializers::bind_stderr(log_file),
	                          bp::initializers::bind_stdout(log_file));
	check_error(error, "program::build(), bp::execute():", "failed in link step");

	int status = wait_for_exit(c, error);
	check_error(error, "program::build(), bp::wait_for_exit()", "failed in link step for file '" + bfs::absolute(lib_path_).string() + "'");

	// NOTE: both check_error calls seem to not catch compilation failure, so as a workaround we use the returned status code
	status = WEXITSTATUS(status); // NOTE: POSIX, see: man 2 waitpid
	if (status != 0) // fail
		detail::log_and_throw(detail::log_level_t::info, "program::build()", "Linking for file '" + lib_path_.string() + "' failed, see build log.");

	built_ = true;
}

const std::string program::get_build_log()
{
	std::stringstream build_log;
	// read files
	for (auto& log_path : log_paths_) {
		build_log << "---- BEGIN: Build log for file: " << log_path.string() << std::endl;
		std::ifstream log_file { log_path.string() };
		if (log_file)
			std::copy(std::istreambuf_iterator<char>(log_file),
			          std::istreambuf_iterator<char>(),
			          std::ostreambuf_iterator<char>(build_log));
		build_log << "---- END: Build log for file: " << log_path.string() << std::endl;
	}

	std::ifstream log_file { link_log_path_.string() };
	if (log_file) {
		build_log << "---- BEGIN: Link log for file: " << lib_path_.string() << std::endl;
		std::copy(std::istreambuf_iterator<char>(log_file),
			          std::istreambuf_iterator<char>(),
			          std::ostreambuf_iterator<char>(build_log));
		build_log << "---- END: Link log for file: " << link_log_path_.string() << std::endl;
	}

	return build_log.str();
}

void* program::get_kernel_ptr(const char* name)
{
	if (built_) {
		// use dlopen, dlsym to get the kernel function by its name and return the code address
		// TODO (maybe): improve resource handling do open the library elsewhere, or lazy (bad for benchmarking) using static, wrap into helper object for calling dlclose, write a generic wrapper that takes two lambdas for handling c resources
		if (!lib_open_) {
			detail::log_stream(detail::log_level_t::info, "program::get_kernel_ptr()")
				<< "Calling dlopen() on: " << lib_path_.string() << std::endl;
			// lib_handle_ = dlmopen(LM_ID_NEWLM, bfs::absolute(lib_path).string().c_str(), RTLD_NOW);
			lib_handle_ = dlopen(bfs::absolute(lib_path_).string().c_str(), RTLD_NOW); // resolve all symbols now, avoid later delays and surprises with unresolved symbols
			lib_open_ = true;

			const char* error_cstr = dlerror();
			if (error_cstr)
				detail::log_and_throw(detail::log_level_t::error, "program::get_kernel_ptr()", std::string { "dlopen() failed with: " } + error_cstr);
		}

		void* kernel_function = dlsym(lib_handle_, name);
		detail::log_stream(detail::log_level_t::info, "program::get_kernel_ptr()")
			<< "Loading symbol '" << name << "' from handle " << lib_handle_ << ", got: " << kernel_function << std::endl;

		const char* error_cstr = dlerror();
		if (error_cstr)
			detail::log_and_throw(detail::log_level_t::error, "program::get_kernel_ptr()", std::string { "dlsym() failed with: " } + error_cstr);

		return kernel_function;
	}
	else {
		detail::log_and_throw(detail::log_level_t::error, "program::get_kernel_ptr()", std::string { "program needs to be build() first." });
		return nullptr;
	}
}

void program::set_lib_path(const std::string& file_name)
{
	lib_path_ = file_name;
	built_ = true;
}

} // namespace kart
