project(jemalloc C)
cmake_minimum_required(VERSION 2.6)

option(DEBUG_MEMORY "Debug memory." OFF)

add_definitions(-DTRIMMED -DMOZ_MEMORY=1 -DMALLOC_OPENTUBE=1)

if(DEBUG_MEMORY)
 add_definitions(-DDEBUG -DMOZ_MEMORY_DEBUG)
else(DEBUG_MEMORY)
 add_definitions(-DNDEBUG)
endif(DEBUG_MEMORY)

if("${CMAKE_C_COMPILER_ID}" MATCHES "clang" OR "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
 set(CLANG_COMPILER 1)
endif("${CMAKE_C_COMPILER_ID}" MATCHES "clang" OR "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")

if(CMAKE_COMPILER_IS_GNUCC OR MINGW OR CYGWIN OR CLANG_COMPILER)
 set(CMAKE_C_FLAGS "-Wno-attributes -Wno-unknown-pragmas")
 set(CMAKE_C_FLAGS_RELEASE "-O3 -g")
 set(CMAKE_C_FLAGS_DEBUG "-ggdb")
endif(CMAKE_COMPILER_IS_GNUCC OR MINGW OR CYGWIN OR CLANG_COMPILER)

if(CLANG_COMPILER)
 set(CMAKE_C_FLAGS "-Wno-unknown-warning-option ${CMAKE_C_FLAGS}")
endif(CLANG_COMPILER)

if(MSVC)
 set(CMAKE_C_FLAGS "")
 set(CMAKE_C_FLAGS_RELEASE "/MD /O2 /Z7")
 set(CMAKE_C_FLAGS_DEBUG "/MDd /Od /Zi")
endif(MSVC)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
 add_definitions(-DMOZ_MEMORY_LINUX=1)
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(CMAKE_SYSTEM_NAME MATCHES ".*BSD")
 add_definitions(-DMOZ_MEMORY_BSD=1)
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
endif(CMAKE_SYSTEM_NAME MATCHES ".*BSD")

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
 add_definitions(-DMOZ_MEMORY_DARWIN=1)
endif(CMAKE_SYSTEM_NAME MATCHES "Darwin")

if(WIN32 OR MINGW)
 add_definitions(-DMOZ_MEMORY_WINDOWS=1)
 set(PLATFORM_EXE_SUFFIX ".exe")
endif(WIN32 OR MINGW)

include(CheckTypeSize)
check_type_size("void *" TYPE_VOID_POINTER_SIZE BUILTIN_TYPES_ONLY)

if(TYPE_VOID_POINTER_SIZE EQUAL 4)
 set(MOZ_MEMORY_SIZEOF_PTR_2POW 2)
elseif(TYPE_VOID_POINTER_SIZE EQUAL 8)
 set(MOZ_MEMORY_SIZEOF_PTR_2POW 3)
elseif(TYPE_VOID_POINTER_SIZE EQUAL 16)
 set(MOZ_MEMORY_SIZEOF_PTR_2POW 4)
endif(TYPE_VOID_POINTER_SIZE EQUAL 4)

add_definitions(-DMOZ_MEMORY_SIZEOF_PTR_2POW=${MOZ_MEMORY_SIZEOF_PTR_2POW})

add_library(jemalloc STATIC jemalloc.c)

enable_testing()
add_executable(test_malloc tests/malloc.c)
add_executable(test_calloc tests/calloc.c)
add_executable(test_realloc tests/realloc.c)
target_link_libraries(test_malloc jemalloc)
target_link_libraries(test_calloc jemalloc)
target_link_libraries(test_realloc jemalloc)
add_test(test_malloc ${CMAKE_CURRENT_BINARY_DIR}/test_malloc${PLATFORM_EXE_SUFFIX})
add_test(test_calloc ${CMAKE_CURRENT_BINARY_DIR}/test_calloc${PLATFORM_EXE_SUFFIX})
add_test(test_realloc ${CMAKE_CURRENT_BINARY_DIR}/test_realloc${PLATFORM_EXE_SUFFIX})
add_custom_target(tests ALL ctest)
