// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_toolset_hpp
#define kart_toolset_hpp

#include <string>

#include <boost/program_options.hpp>

namespace kart {

/**
 * Toolset abstraction
 * A toolset describes a compiler and linker executable and their respective
 * options.
 * This class is default copy- and assignable.
 */
class toolset {
public:
	// default ctor searches environment and standard paths for the default tool chain
	toolset();

	// specifiy configuration
	toolset(const std::string& config_file_path);
	
	// getters
	const std::string& compiler_exe() const { return compiler_exe_; }
	const std::string& linker_exe() const { return linker_exe_; }
	const std::string& compiler_options_always() const { return compiler_options_always_; }
	const std::string& source_file_extension() const { return source_file_extension_; }

	const std::string& linker_options_always() const { return linker_options_always_; }
	const std::string& compiler_options_default() const { return compiler_options_default_; }
	const std::string& linker_options_default() const { return linker_options_default_; }

	const std::string& compiler_options() const { return compiler_options_; }
	const std::string& linker_options() const { return linker_options_; }

	// modifiers
	void set_compiler_options(const std::string& options) { compiler_options_ = options; }
	void append_compiler_options(const std::string& options) { compiler_options_ += options; }

	void set_linker_options(const std::string& options) { linker_options_ = options; }
	void append_linker_options(const std::string& options) { linker_options_ += options; }

private:
	void parse_options(const std::string& config_file_path);

	std::string compiler_exe_;
	std::string linker_exe_;
	std::string compiler_options_always_;
	std::string source_file_extension_;
	
	std::string linker_options_always_;
	std::string compiler_options_default_;
	std::string linker_options_default_;
	
	// not parsed, to set by user
	// NOTE: options are composed of the options_always config and the options which are initialised with options_default
	std::string compiler_options_;
	std::string linker_options_;
};

} // namespace kart

#endif // kart_toolset_hpp
