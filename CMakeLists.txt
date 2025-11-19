cmake_minimum_required(VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_GTEST "Whether to build Google Test" ON) 

add_executable(${PROJECT_NAME} 
    main.cpp
    ip_filter.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WITH_GTEST)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
        message(STATUS "Downloading googletest...")
        
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
        )
        
        FetchContent_MakeAvailable(googletest)
        
    else()
        add_subdirectory(googletest)
    endif()
    
    message(STATUS "Google Test configured")
    
    add_executable(test_ip_filter 
        test_ip_filter.cpp
        ip_filter.cpp                 
    )
    
    set_target_properties(test_ip_filter PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_link_libraries(test_ip_filter PRIVATE gtest_main gmock)
    
    if(MSVC)
        target_compile_options(test_ip_filter PRIVATE /W4)
    else()
        target_compile_options(test_ip_filter PRIVATE -Wall -Wextra -Wpedantic)
    endif()

endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "oflika256")
set(CPACK_PACKAGE_CONTACT "oflika256@gmail.com")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oflika256 <oflika256@gmail.com>")

include(CPack)

if(WITH_GTEST)
    enable_testing()
    add_test(NAME ip_filter_tests COMMAND test_ip_filter)
endif()
