// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/kart.hpp"

#include <iostream>

//#include <noma/bmt/bmt.hpp>

// kernel type: return_type(*)(argument_types, ...)
using my_kernel_t = double(*)(double, size_t, double*);

// inline C++ code as c-string (character array) literal (C++11 raw string literal R"()")
const char my_kernel_src[] = R"kart_src(
#include <stdio.h>
extern "C" {
double my_kernel(double a, size_t v_size, double* v)
{
	double sum = 0.0;
	for (size_t i = 0; i < v_size; ++i)
	{
		v[i] = a * v[i];
		sum += v[i];
	}
	return sum;
}
}
)kart_src";

int main(int argc, char** argv)
{
	// kart API usage example:
	kart::program my_kernel_prog(my_kernel_src);

	kart::toolset ts; // create a default toolset
	ts.append_compiler_options(" -DSOME_CONSTANT=5.0");

	//my_kernel_prog.build(); // build with default toolset
	try {
		my_kernel_prog.build(ts); // build with custom toolset
	} catch (...) {
		std::cout << "Build failed, here is the log:" << std::endl;
		std::cout << my_kernel_prog.get_build_log();
		return 1;
	}
	using my_kernel_t = double(*)(double, size_t, double*);
	auto my_kernel = my_kernel_prog.get_kernel<my_kernel_t>("my_kernel");
	
	// use kernel as usual
	double a = {2.0};
	double v[] {1.0, 2.0, 3.0};
	
	std::cout << "sizeof(v) = " << sizeof(v)/sizeof(double) << std::endl;
	double ret = my_kernel(a, sizeof(v)/sizeof(double), &v[0]);
	std::cout << "ret = " << ret << std::endl;
	
	return 0;
}
