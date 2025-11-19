cmake_minimum_required(VERSION 3.14)
project(IPFilter VERSION 1.0.0 LANGUAGES CXX)

# Настройка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции проекта
option(BUILD_TESTS "Build tests" ON)

# Настройки компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
    endif()
endif()

# Основное приложение
add_executable(ip_filter
    main.cpp
    ip_filter.cpp
    ip_filter.h
)

# Тесты (если включены)
if(BUILD_TESTS)
    # Поиск GTest
    find_package(GTest)
    
    if(GTest_FOUND)
        add_executable(ip_filter_tests
            test_ip_filter.cpp
            ip_filter.cpp
            ip_filter.h
        )
        
        target_link_libraries(ip_filter_tests 
            GTest::gtest 
            GTest::gtest_main
        )
        
        # Добавление тестов в CTest
        include(GoogleTest)
        gtest_discover_tests(ip_filter_tests)
        
        message(STATUS "Tests enabled")
    else()
        message(WARNING "GTest not found, tests disabled")
    endif()
endif()

# Настройка CPack для создания .deb пакета (опционально)
if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME "ip-filter")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Auto Build")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "IP address filtering tool")
    set(CPACK_DEBIAN_PACKAGE_SECTION "net")
    
    install(TARGETS ip_filter
        RUNTIME DESTINATION bin
    )
    
    include(CPack)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_test(test_ip_filter test_ip_filter)
endif()

# Информация о конфигурации
message(STATUS "IP Filter Project Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
