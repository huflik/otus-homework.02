cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Google Test" ON) 

add_executable(${PROJECT_NAME} 
    main.cpp
    ip_filter.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


if(WITH_GTEST)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
        message(STATUS "Downloading googletest...")
        execute_process(
            COMMAND git clone --depth 1 https://github.com/google/googletest.git -b v1.17.0
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE result
        )
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to download googletest")
        endif()
    endif()
    
    add_subdirectory(googletest)
    
    message(STATUS "Google Test configured via add_subdirectory")
    
    add_executable(test_ip_filter 
        test_ip_filter.cpp
        ip_filter.cpp                 
    )
    
    set_target_properties(test_ip_filter PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(test_ip_filter PRIVATE ${GTEST_INCLUDE_DIR})
   
    target_link_libraries(test_ip_filter gtest_main)

endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT oflika256@gmail.com)
include(CPack)

if(WITH_GTEST)
    enable_testing()
    add_test(test_ip_filter test_ip_filter)
endif()
