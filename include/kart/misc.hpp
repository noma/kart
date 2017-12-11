// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef kart_misc_hpp
#define kart_misc_hpp

#include <string>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

namespace kart {

namespace bp = ::boost::process;
namespace bfs = ::boost::filesystem;
namespace bs = ::boost::system;

bool check_error(const bs::error_code& ec, const std::string& location, const std::string& msg);

} // namespace kart

#endif // kart_misc_hpp
