# Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

option(KART_BUILD_EXAMPLES "Build example applications" OFF)

if(KART_BUILD_EXAMPLES)
	add_executable(example example.cpp)
	target_link_libraries(example ${LIBRARY_NAME} ${Boost_LIBRARIES})

	add_executable(examplec example.c)
	target_link_libraries(examplec ${LIBRARY_NAME_C} ${Boost_LIBRARIES})

	if(KART_ENABLE_FORTRAN)
		add_executable(examplef example.F90)
		target_link_libraries(examplef ${LIBRARY_NAME_C} ${Boost_LIBRARIES})
		target_include_directories(examplef PRIVATE ../fortran)
	endif()
endif()

# see CMakeLists.txt in src
add_subdirectory(kart)
