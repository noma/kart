# Copyright (c) 2016-2017 Matthias Noack (ma.noack.pr@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

MESSAGE("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
MESSAGE("CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})

# C++ library
set(KART_SRC constants.cpp log.cpp log_level.cpp misc.cpp program.cpp toolset.cpp)
message("LIBRARY_NAME: " STATIC ${LIBRARY_NAME})
add_library(${LIBRARY_NAME} STATIC ${KART_SRC})
target_include_directories(${LIBRARY_NAME} PUBLIC include)
target_link_libraries(${LIBRARY_NAME} dl ${Boost_LIBRARIES})

# C wrapper library
message("LIBRARY_NAME_C: " ${LIBRARY_NAME_C})
#add_library(${LIBRARY_NAME_C} STATIC ${KART_SRC} c_api.cpp)
add_library(${LIBRARY_NAME_C} STATIC c_api.cpp)
target_include_directories(${LIBRARY_NAME_C} PUBLIC include)
target_link_libraries(${LIBRARY_NAME_C} dl ${Boost_LIBRARIES} ${LIBRARY_NAME})

# Fortran Module
#if(KART_ENABLE_FORTRAN)
#	add_library(kartf ../../fortran/kart.F90)
#	target_link_libraries(kartf ${LIBRARY_NAME_C} ${Boost_LIBRARIES})
#	#target_include_directories(examplef PRIVATE ../fortran)
#endif()
