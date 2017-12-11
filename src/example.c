// Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "kart/kart.h" // NOTE: C-API

#include <stdio.h>
#include <string.h>

// inline C code as C-string (C does not support C++11 raw string literals)
// manual line breaks or some escaping are required, source files are more convenient here
const char my_kernel_src[] = "                       \
#include <stdio.h>\n                                 \
double my_kernel(double a, size_t v_size, double* v) \
{                                                    \
	double sum = 0.0;                                \
	for (size_t i = 0; i < v_size; ++i)              \
	{                                                \
		v[i] = a * v[i];                             \
		sum += v[i];                                 \
	}                                                \
	return sum;                                      \
}                                                    \
";

int main(int argc, char** argv)
{
	int ts_handle = kart_create_default_toolset();
	kart_append_compiler_options(ts_handle, " -DSOME_CONSTANT=5.0");

	int prog_handle = kart_create_program(my_kernel_src, strlen(my_kernel_src));
	kart_build_program(prog_handle, ts_handle);

	printf("Build log\n%s\n", kart_get_build_log(prog_handle));

	void* kernel_ptr = kart_get_kernel(prog_handle, "my_kernel");

	double(*my_kernel)(double, size_t, double*) = kernel_ptr;

	double a = {2.0};
	double v[] = {1.0, 2.0, 3.0};

	printf("sizeof(v) = %lu\n", sizeof(v) / sizeof(double));
	double ret = my_kernel(a, sizeof(v) / sizeof(double), &v[0]);
	printf("ret = %f\n", ret);

	printf("Binary is located at: %s\n", kart_get_binary_file_name(prog_handle));

	// reload same program and kernel from binary
	int prog_handle_binary = kart_create_program_from_binary_file(kart_get_binary_file_name(prog_handle));

	// destroy original program
	kart_delete_program(prog_handle); // closes lib handle

	void* kernel_ptr_binary = kart_get_kernel(prog_handle_binary, "my_kernel"); // opens lib handle on first call after build or creation from binary
	double(*my_kernel_binary)(double, size_t, double*) = kernel_ptr_binary;
	double ret_binary = my_kernel_binary(a, sizeof(v)/sizeof(double), &v[0]);
	printf("ret_binary = %f\n", ret_binary);

	return 0;
}
