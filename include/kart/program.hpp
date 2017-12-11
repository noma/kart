// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_program_hpp
#define kart_program_hpp

#include "kart/toolset.hpp"

#include <string>

#include <boost/filesystem.hpp>

namespace kart {

/**
 * Program abstraction
 * This class represents a program, i.e. source code that can be compiled
 * and linked using a toolset. Callable kernels (or functions can be queried
 * for execution.
 * This class can only be moved, but not copied, as it is linked to with
 * resources like files and open library handles.
 */
class program {
public:
	program(const program&) = delete;
	program& operator=(const program&) = delete;
	program(program&&) = default; // move only
	program& operator=(program&&) = default;

	// single source file ctors
	program(const char* src, size_t size);
	program(const std::string& src);
	~program();
	// TODO: multi-source file ctors
	// TODO: alternative: add source files file by file? (would there be any advantage?)

	void build(const toolset& tc = kart::toolset());
	const std::string get_build_log();
	
	template<typename T = void*>
	T get_kernel(const std::string& name) {
		return reinterpret_cast<T>(get_kernel_ptr(name.c_str()));
	}

	const std::string& tmp_path() const { return tmp_path_.string(); }
	const std::string& binary_file_name() const { return lib_path_.string(); }
	void set_binary_file_name(const std::string& file_name);

	static program create_from_src_file(const std::string& file_name);
	static program create_from_binary_file(const std::string& file_name);

private:
	program() = default; // used by static create_* member functions

	void* get_kernel_ptr(const char* name);
	void set_lib_path(const std::string& file_name);

	boost::filesystem::path tmp_path_;
	std::vector<boost::filesystem::path> src_paths_;
	std::vector<boost::filesystem::path> obj_paths_;
	std::vector<boost::filesystem::path> log_paths_;
	boost::filesystem::path link_log_path_;
	bool built_ = false;

	boost::filesystem::path lib_path_;
	void* lib_handle_ = nullptr;
	bool lib_open_ = false;
};

} // namespace kart

#endif // kart_program_hpp
