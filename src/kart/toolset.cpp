// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/toolset.hpp"

#include "kart/constants.hpp"
#include "kart/log.hpp"

#include <iostream>
#include <fstream>

namespace kart {

toolset::toolset()
{
	// TODO (maybe): introduce default search paths and names
	// TODO (maybe): give toolsets names and store them system-wide for the installed compilers, use some map name->toolset for selection

	const char* config_file_path = std::getenv(constants::env_var_default_toolset_config);
	if (config_file_path) {
		// parse from environment
		parse_options(config_file_path);
	}
	else
	{
		detail::log_and_throw(detail::log_level_t::error, "toolset::toolset()", std::string { "No default toolset config file found in environment, set " } + constants::env_var_default_toolset_config);
	}
}

toolset::toolset(const std::string& config_file_path)
{
	parse_options(config_file_path);
}

void toolset::parse_options(const std::string& config_file_path)
{
	// command line options
	boost::program_options::options_description desc("Supported options");
	desc.add_options()
		("compiler.exe", boost::program_options::value(&compiler_exe_)->default_value(compiler_exe_), "absolute path to compiler executable")
		("compiler.options-always", boost::program_options::value(&compiler_options_always_)->default_value(compiler_options_always_), "mandatory command line options for the compiler")
		("compiler.options-default", boost::program_options::value(&compiler_options_default_)->default_value(compiler_options_default_), "default command line options for the compiler")
		("compiler.source-file-extension", boost::program_options::value(&source_file_extension_)->default_value(source_file_extension_), "file extension used for the temporary source file")
		("linker.exe", boost::program_options::value(&linker_exe_)->default_value(linker_exe_), "absolute path to linker executable")
		("linker.options-always", boost::program_options::value(&linker_options_always_)->default_value(linker_options_always_), "mandatory command line options for the linker")
		("linker.options-default", boost::program_options::value(&linker_options_default_)->default_value(linker_options_default_), "default command line options for the linker")
	;

	boost::program_options::variables_map vm;
	// NOTE: no try-catch here to avoid exceptions, that cause offload-dependencies to boost exception in the MIC code
	try {
		detail::log(detail::log_level_t::info, "toolset::parse_options()", "Parsing options from file " + config_file_path);

		std::ifstream config_file(config_file_path);
		boost::program_options::store(boost::program_options::parse_config_file(config_file, desc), vm);
		boost::program_options::notify(vm);
	} catch (...) {
		std::ostringstream desc_ss;
		desc_ss << desc;
		detail::log_and_throw(detail::log_level_t::error, "toolset::parse_options()", "Caught exception while parsing toolset config file, check file format:\n" + desc_ss.str());
	}

	// set default options
	compiler_options_ = compiler_options_default_;
	linker_options_ = linker_options_default_;
}

} // namespace kart
