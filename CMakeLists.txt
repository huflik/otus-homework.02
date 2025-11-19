cmake_minimum_required(VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_GTEST "Whether to build Google Test" ON)
option(BUILD_TESTING "Build tests" ${WITH_GTEST})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(ip_filter_lib STATIC
    src/ip_filter.cpp
)

target_include_directories(ip_filter_lib 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ip_filter
    src/main.cpp
)

target_link_libraries(ip_filter ip_filter_lib)

add_subdirectory(test)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "oflika256")
set(CPACK_PACKAGE_CONTACT "oflika256@gmail.com")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oflika256 <oflika256@gmail.com>")

include(CPack)
