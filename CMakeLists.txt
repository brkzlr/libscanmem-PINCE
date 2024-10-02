cmake_minimum_required(VERSION 3.22)
project(libscanmem LANGUAGES C)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.c)
add_library(scanmem SHARED ${SOURCES})

set_target_properties(scanmem
	PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
		EXPORT_COMPILE_COMMANDS ON
)

target_compile_options(scanmem PRIVATE -Wall)
target_compile_definitions(scanmem PRIVATE _POSIX_C_SOURCE=200809L)
if(${CMAKE_C_BYTE_ORDER} STREQUAL "BIG_ENDIAN")
	target_compile_definitions(scanmem PRIVATE IS_BIG_ENDIAN)
endif()

# Libscanmem requirements
include(CMakePushCheckState)
cmake_push_check_state()
set(CMAKE_REQUIRED_FLAGS "-std=c11")
set(CMAKE_REQUIRED_DEFINITIONS "-D_POSIX_C_SOURCE=200809L")

include(CheckIncludeFiles)
check_include_files("stdbool.h;fcntl.h;limits.h;stddef.h;sys/ioctl.h;sys/time.h" HAVE_REQ_INCLUDES)
if(NOT HAVE_REQ_INCLUDES)
	message(FATAL_ERROR "Missing required includes!")
endif()

include(CheckSymbolExists)
check_symbol_exists(malloc "stdlib.h" HAVE_MALLOC)
if(NOT HAVE_MALLOC)
	message(FATAL_ERROR "malloc not found! Aborting.")
endif()
check_symbol_exists(realloc "stdlib.h" HAVE_REALLOC)
if(NOT HAVE_REALLOC)
	message(FATAL_ERROR "realloc not found! Aborting.")
endif()
check_symbol_exists(alloca "alloca.h" HAVE_ALLOCA)
if(NOT HAVE_ALLOCA)
	message(FATAL_ERROR "alloca not found! Aborting.")
endif()
check_symbol_exists(strtod "stdlib.h" HAVE_STRTOD)
if(NOT HAVE_STRTOD)
	message(FATAL_ERROR "strtod not found! Aborting.")
endif()
check_symbol_exists(getline "stdio.h" HAVE_GETLINE)
if(NOT HAVE_GETLINE)
	message(FATAL_ERROR "getline not found! Aborting.")
endif()
check_symbol_exists(popen "stdio.h" HAVE_POPEN) # If popen exists, no reason to also check for pclose
if(NOT HAVE_POPEN)
	message(FATAL_ERROR "popen not found! Aborting.")
endif()
check_symbol_exists(strdup "string.h" HAVE_STRDUP)
if(NOT HAVE_STRDUP)
	message(FATAL_ERROR "strdup not found! Aborting.")
endif()
cmake_pop_check_state()

if(NOT APPLE)
	if(NOT EXISTS "/proc/self/maps")
		message(FATAL_ERROR "This system does not seem to have /proc/pid/maps files.")
	elseif(NOT EXISTS "/proc/self/mem")
		message(FATAL_ERROR "This system does not seem to have /proc/pid/mem files.")
	endif()
endif()
