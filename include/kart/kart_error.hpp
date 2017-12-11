// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace kart {

class kart_error : public std::runtime_error
{
public:
	kart_error (const std::string& msg) : runtime_error(msg) {};
};

} // namespace kart